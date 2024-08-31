#include"event_loop.h"

event_loop::event_loop()
{
	_epfd=epoll_create1(0);
	if(_epfd==-1){
		fprintf(stderr,"epoll_createq error\n");
		exit(1);
	}

}
//阻塞循环监听事件
void event_loop:: event_process()
{
	io_event_map_it ev_it;
	while(true){
		//printf("wait IN OUT event...\n");
/*		for(listen_fd_set::iterator it=listen_fds.begin();it!=listen_fds.end();it++){
			printf("fd %dis listening by event_loop...\n",*it);
		}
*/
		int nfds=epoll_wait(_epfd,_fired_evs,MAXEVENTS,-1);
		for(int i=0;i<nfds;i++){
			//1通过epoll触发的fd 从map中找打对应的io_event
			ev_it=_io_evs.find(_fired_evs[i].data.fd);

			//取出对应的事件
			io_event *ev=&(ev_it->second);

			if(_fired_evs[i].events&EPOLLIN){
				//回调参数
				void *args=ev->rcb_args;
				//读事件 调用读回调函数
				ev->read_callback(this,_fired_evs[i].data.fd,args);
			}
			else if(_fired_evs[i].events&EPOLLOUT){
				//回调参数
				void *args=ev->wcb_args;
				//写事件 调用写回调函数
				ev->write_callback(this,_fired_evs[i].data.fd,args);
			}
			else if(_fired_evs[i].events&(EPOLLHUP | EPOLLERR)){
				//水平触发未处理,可能会出现HUP事件，需要正常处理读写
				if(ev->read_callback!=NULL){
					//回调参数
					void *args=ev->rcb_args;
					//读回调业务
					ev->read_callback(this,_fired_evs[i].data.fd,args);
				}
				else if(ev->write_callback!=NULL){
					void *args=ev->wcb_args;
					ev->write_callback(this,_fired_evs[i].data.fd,args);
				}
				else{
					//删除
					fprintf(stderr,"fd %d get error,delete from epoll",_fired_evs[i].data.fd);
					this->del_io_event(_fired_evs[i].data.fd);
				}
			}
		}
		//每次全部的fd读写时间执行完毕
		//在此处 执行一些 其它的task任务流程
		this->execute_ready_tasks();
	}
}
//添加一个io事件到event_loop中
void event_loop::add_io_event(int fd,io_callback *proc,int mask,void *args)
{
	int op;
	int final_mask;
	//1找到当前的fd 是否是已有事件
	io_event_map_it it=_io_evs.find(fd);
	if(it==_io_evs.end()){
		//如果没有 以ADD的方式添加到epoll中
		op=EPOLL_CTL_ADD;
		final_mask=mask;
	}else{
		op=EPOLL_CTL_MOD;
		final_mask=it->second.mask | mask;
	}

	//2将fd和io_callback绑定 就是添加到map中
	if(mask&EPOLLIN){
		//该事件是一个读事件
		_io_evs[fd].read_callback=proc;//注册读回调业务
		_io_evs[fd].rcb_args=args;
	}else if(mask&EPOLLOUT){
		_io_evs[fd].write_callback=proc;
		_io_evs[fd].wcb_args=args;
	}
	
	_io_evs[fd].mask=final_mask;

	//将io事件添加到原生epoll中
	struct epoll_event event;
	event.events=final_mask;
	event.data.fd=fd;
	if(epoll_ctl(_epfd,op,fd,&event)==-1){
		fprintf(stderr,"epoll_ctl error\n");
		return;
	}

	//将当前fd加入到正在监听的fd集合中
	listen_fds.insert(fd);

}
//删除一个事件从event_loop中
void event_loop::del_io_event(int fd)
{
	io_event_map_it it=_io_evs.find(fd);
	if(it==_io_evs.end()){
		return;
	}
	//将事件从map中删除
	_io_evs.erase(fd);
	//将事件从set中删除
	listen_fds.erase(fd);
	//将事件从原生的epoll_event中删除
	epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,NULL);
}

//删除一个事件的某触发条件
void event_loop::del_io_event(int fd,int mask)
{
	io_event_map_it it=_io_evs.find(fd);
	if(it==_io_evs.end()){
		return;
	}
	int &o_mask=it->second.mask;
	o_mask=o_mask&(~mask);
	if(o_mask==0){
		//通过删除条件已经没有删除条件
		this->del_io_event(fd);
	}
	else{
		//如果事件还存在
		struct epoll_event event;
		event.events=o_mask;
		event.data.fd=fd;
		epoll_ctl(_epfd,EPOLL_CTL_MOD,fd,&event);
	}

}
//添加一个task任务到_ready_tasks集合中
void event_loop::add_task(task_func func,void *args)
{
	task_func_pair func_pair(func,args);
	_ready_tasks.push_back(func_pair);

}
//执行全部的ready_tasks里面的全部任务
void event_loop::execute_ready_tasks()
{
	//遍历_ready_tasks 取出每个任务去执行
	std::vector<task_func_pair>::iterator it;
	for(it=_ready_tasks.begin();it!=_ready_tasks.end();it++){
		task_func func = it->first;//取出任务的执行函数
		void *args=it->second;
		//调用执行函数
		func(this,args);
	}
	//全部函数执行完毕，清空当前_ready_tasks集合
	_ready_tasks.clear();

}









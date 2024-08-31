#include"thread_pool.h"

thread_queue<task_msg> *thread_pool::get_thread(){
		
	if(_index==_thread_cnt){
		_index=0;

	}
	
	printf("======>thread_num = %d<=====\n",_index);
	return _queues[_index++];
}


//一旦由task任务消息过来 这个业务函数就会被loop函数监听到并执行
//读取消息队列中的消息 并处理
void deal_task(event_loop *loop,int fd,void *args){
	//从thread_queue中取出数据
	thread_queue<task_msg>* queue=(thread_queue<task_msg>*)args;

	//取出queue调用recv方法
	std::queue<task_msg> new_task_queue;
	queue->recv(new_task_queue);//此时就是存放的thread_queue的全部任务

	while(new_task_queue.empty()!=true){
		//从队列中得到一个任务 .
		task_msg task=new_task_queue.front();
		//将这个任务从队列中弹出
		new_task_queue.pop();
		if(task.type==task_msg::NEW_CONN){
			//任务1
			tcp_conn *conn=new tcp_conn(task.connfd,loop);
			//让当前线程去创建一个链接，同时将这个链接的connfd加入到当前的thread的event_loop中
			if(conn==NULL){
				fprintf(stderr,"in thread new tcp_conn error\n");
				exit(1);
			}
			printf("[thread]:create net connection succ!\n");
		}
		else if(task.type==task_msg::NEW_TASK){
			//任务2 是一个普通的任务 
			//将该任务添加到event_loop循环中 去执行
			loop->add_task(task.task_cb,task.args);
		}
		else{
			fprintf(stderr,"unknow task\n");
		}
	}

	//判断task任务类型

}

void *thread_main(void *args){
	thread_queue<task_msg> *queue=(thread_queue<task_msg>*)args;
	event_loop *loop=new event_loop();
	if(loop==NULL){
		fprintf(stderr,"new event_loop error");
		exit(1);
	}
	queue->set_loop(loop);
	queue->set_callabck(deal_task,queue);

	//启动loop监听
	loop->event_process();
	return NULL;
}

//初始化线程池的构造函数  每次创建一个server就应该创建一个线程池
thread_pool::thread_pool(int thread_cnt){
	_queues=NULL;
	_index=0;
	_thread_cnt=thread_cnt;
	if(_thread_cnt<=0){
		fprintf(stderr,"thread_cnt need>0\n");
		exit(1);
	}

	//创建thread_queue
	_queues=new thread_queue<task_msg>*[thread_cnt];//开辟了thread_cnt个指针 每个指针并没有new一个对象
	_tids=new pthread_t[thread_cnt];

	//开辟线程
	int ret;
	for(int i=0;i<thread_cnt;i++){
		//给一个thread_queue开辟内存 创建对象
		_queues[i]=new thread_queue<task_msg>();
		//第i个线程 绑定第i个thread_queue
		ret=pthread_create(&_tids[i],NULL,thread_main,_queues[i]);
		if(ret==-1){
			perror("thread_pool create error");
			exit(1);
		}

		printf("create %d thread\n",i);

		//将线程设置为detch线程分离模式
		pthread_detach(_tids[i]);
	}

	return ;
}

//发送一个task NEW_TASK的任务接口
void thread_pool::send_task(task_func func,void *args){
	//给当前thread_pool中每一个thread中的thread_queue发送一个他task消息
	task_msg task;
	//制作一个任务
	task.type=task_msg::NEW_TASK;
	task.task_cb=func;
	task.args=args;
		
	for(int i=0;i<_thread_cnt;i++){
		thread_queue<task_msg> *queue=_queues[i];
		//给queue发送一个task任务
		queue->send(task);
	}


}

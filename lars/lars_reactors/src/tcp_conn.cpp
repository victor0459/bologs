#include "tcp_conn.h"
#include<netinet/in.h>
#include<netinet/tcp.h>
#include"message.h"
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include"tcp_server.h"

void conn_rd_callback(event_loop *loop,int fd,void *args)
{
	tcp_conn *conn=(tcp_conn*)args;
	conn->do_read();
}
void conn_wt_callback(event_loop *loop,int fd,void *args)
{
	tcp_conn *conn=(tcp_conn*)args;
	conn->do_write();
}
tcp_conn::tcp_conn(int connfd,event_loop *loop)
{
	_connfd=connfd;
	_loop=loop;
	//注意此处需要将connfd设置为非阻塞状态 x
	int flag=fcntl(_connfd,F_SETFL,0);
	fcntl(_connfd,F_SETFL,O_NONBLOCK | flag);

	//设置TCP_NODELAY状态，禁止读写缓存，降低小包延迟
	int op=1;
	setsockopt(_connfd,IPPROTO_TCP,TCP_NODELAY,&op,sizeof(op));
	
	//3执行创建连接成功之后要触发的Hook函数
	if(tcp_server::conn_start_cb!=NULL){
		tcp_server::conn_start_cb(this,tcp_server::conn_start_cb_args);
	}

	//将tcp_conn读事件 添加到loop监听
	_loop->add_io_event(_connfd,conn_rd_callback,EPOLLIN,this);
	
	//将自己添加到tcp_server中的conns集合中
	tcp_server::increase_conn(_connfd,this);
}

void tcp_conn::do_read()
{
	int ret=ibuf.read_data(_connfd);
	if(ret==-1){
		fprintf(stderr,"read data from socket\n");
		this->clean_conn();
		return;		
	}
	else if(ret==0){
		//对端正常关闭
		printf("peer client closed!\n");
		this->clean_conn();
	}

	//读出来的消息头
	msg_head head;
	while(ibuf.length()>=MESSAGE_HEAD_LEN){
		//2.1先读取头部得到msgid msglen
		memcpy(&head,ibuf.data(),MESSAGE_HEAD_LEN);
		if(head.msglen>MESSAGE_LENGTH_LIMIT || head.msglen<0){
			fprintf(stderr,"data format error,too large or too small,need close\n");
			this->clean_conn();
			break;
		}
		//2.2判断得到的消息体长度和头部里的长度是否一致
		if(ibuf.length()<MESSAGE_HEAD_LEN+head.msglen){
			//缓存中buf剩余的数据 小于应该接收到的数据
			break;
		}
		//表示当前包是合法的
		ibuf.pop(MESSAGE_HEAD_LEN);
		

		//针对不同的msgid，来调不同的业务
		tcp_server::router.call(head.msgid,head.msglen,ibuf.data(),this);
		//整个消息都处理完了
		ibuf.pop(head.msglen);
	}
	ibuf.adjust();
	return ;
}

void tcp_conn::do_write()
{
	while(obuf.length()){
		int write_num=obuf.write2fd(_connfd);
		if(write_num==-1){
			fprintf(stderr,"tcp_conn write connfd\n");
			this->clean_conn();
			return ;
		}
		else if(write_num==0){
			//当前不可写
			break;
		}
		if(obuf.length()==0){
			//数据已经全部写完，将_connfd的写事件删除
			_loop->del_io_event(_connfd,EPOLLOUT);
		}

	}
	return ;
}

int tcp_conn::send_message(const char *data,int msglen,int msgid)
{
	//printf("server send message:data:%s,msglen:%d,msgid:%d\n",data,msglen,msgid);
	
	bool active_epollout=false;
	if(obuf.length()==0){
		active_epollout=true;
	}

	//组装消息头
	msg_head head;
	head.msgid=msgid;
	head.msglen=msglen;
	int ret=obuf.send_data((const char*)&head,MESSAGE_HEAD_LEN);
	if(ret!=0){
		fprintf(stderr,"send head error\n");
		return -1;
	}
	//将消息体放入obuf中
	ret=obuf.send_data(data,msglen);
	if(ret!=0){
		fprintf(stderr,"send data error\n");
		//如多消息体写失败，消息头需要弹出重置
		obuf.pop(MESSAGE_HEAD_LEN);
		return -1;
	}
	//将_connfd添加一个写事件EPOLLOUT回调，让回调直接将obuf中的数据写到对端
	if(active_epollout==true){
		_loop->add_io_event(_connfd,conn_wt_callback,EPOLLOUT,this);
	}
	return 0;

}

void tcp_conn::clean_conn()
{
	//调用连接销毁之前要触发的hook函数
	if(tcp_server::conn_close_cb!=NULL){
		tcp_server::conn_close_cb(this,tcp_server::conn_close_cb_args);
	}


	//将tcp_server中 把当前连接删除
	tcp_server::decrease_conn(_connfd);
	//连接的清理工作
	_loop->del_io_event(_connfd);

	ibuf.clear();
	obuf.clear();

	close(_connfd);
}









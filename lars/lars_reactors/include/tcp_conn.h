#pragma once
#include "reactor_buf.h"
#include "event_loop.h"
#include"net_connection.h"

class tcp_conn:public net_connection
{
public:
	tcp_conn(int connfd,event_loop *loop);

	//被动处理读业务方法 被event_loop监听 读
	void do_read();

	//被动处理写业务方法 被event_loop监听
	void do_write();

	//主动发送消息的方法
	virtual int send_message(const char *data,int msglen,int msgid);

	virtual int get_fd(){
		return this->_connfd;
	}

	//销毁当前连接
	void clean_conn();

private:
	int _connfd;

	//当前connfd被哪个event_loop所监听
	event_loop *_loop;

	//输出的output_buf
	output_buf obuf;

	//输入的input_buf
	
	input_buf ibuf;
};

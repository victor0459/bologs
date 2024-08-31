#pragma once

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"reactor_buf.h"
#include"event_loop.h"
#include"message.h"
#include"net_connection.h"


class tcp_client:public net_connection
{
	public:
		tcp_client(event_loop *loop,const char *ip,unsigned short port);

		//1发送方法
		virtual int send_message(const char*data,int msglen,int msgid);

		virtual int get_fd(){
			return this->_sockfd;
		}

		//处理读业务
		void do_read();

		//处理写业务
		void do_write();

		//释放连接
		void clean_conn();

		//连接服务器
		void do_connect();

		void add_msg_router(int msgid,msg_callback *cb,void *user_data=NULL){
			_router.register_msg_router(msgid,cb,user_data);
		}

		//输入缓冲
		input_buf ibuf;
		//输出缓冲
		output_buf obuf;
		//server端的ip地址
		struct sockaddr_in _server_addr;
		socklen_t _addrlen;

		void set_conn_start(conn_callback cb,void *args=NULL){
			_conn_start_cb=cb;
			_conn_start_cb_args=args;
		}
		void set_conn_close(conn_callback cb,void *args=NULL){
			_conn_close_cb=cb;
			_conn_close_cb_args=args;
		}      


		//创建连接之后触发的回调函数
		conn_callback _conn_start_cb;
		void * _conn_start_cb_args;
		//销毁连接之前触发的回调函数
		conn_callback _conn_close_cb;
		void *_conn_close_cb_args;


	private:

		int _sockfd;//当前客户链接

		//客户端事件的处理机制
		event_loop *_loop;

		//客户端处理服务器消息的回调业务函数

		//msg_callback *_msg_callback;

		//消息路由分发机制
		msg_router _router;	
};

#pragma once
#include<netinet/in.h>
#include<stdio.h>
#include"event_loop.h"
#include"tcp_conn.h"
#include"message.h"
#include"thread_pool.h"
class tcp_server{
	public:
		//server的构造函数
		tcp_server(event_loop * loop,const char*ip,uint16_t port);
		//开始提供链接服务
		void do_accept();
		//连接对象释放的析构
		~tcp_server();
	private:
		int _sockfd;//套接字
		struct sockaddr_in _connaddr;//客户端连接地址
		socklen_t _addrlen;//客户端连接地址长度
		event_loop *_loop;
	public:
		static tcp_conn **conns;//全部已经在线的连接集合
		static void increase_conn(int connfd,tcp_conn *conn);//新增一个链接
		static void decrease_conn(int connfd);//减少一个链接
		static void get_conn_num(int *curr_conn);
		static pthread_mutex_t _conns_mutex;

#define MAX_CONNS 3//从配置文件中读取 TODO

		static int _max_conns;//最大的连接个数
		static int _curr_conns;//当前所管理的连接个数		
		

		//添加一个路由分发机制句柄
		static msg_router router;
		//提供一个添加路由的方法
		void add_msg_router(int msgid,msg_callback *cb,void *user_data=NULL){
			router.register_msg_router(msgid,cb,user_data);	
		}
		

		//设置连接创建之后的hook函数
		static void set_conn_start(conn_callback cb,void *args=NULL){
			conn_start_cb=cb;
			conn_start_cb_args=args;
		}
		//设置连接销毁之前的hook函数
		static void set_conn_close(conn_callback cb,void *args=NULL){
			conn_close_cb=cb;
			conn_close_cb_args=args;
		}
		
		//创建连接之后 要触发的回调函数
		static conn_callback conn_start_cb;
		static void * conn_start_cb_args;
		//销毁链接之前 要触发的回调函数
		static conn_callback conn_close_cb;
		static void * conn_close_cb_args;
		
		//提供一个获取thread_pool的方法
		thread_pool *get_thread_pool(){
			return _thread_pool;
		}

		private:
		thread_pool *_thread_pool;
};

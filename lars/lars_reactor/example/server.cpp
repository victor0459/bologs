#include "tcp_server.h"
#include<string.h>
#include"config_file.h"
tcp_server *server;
//当前任务的一个task callback
void print_lars_task(event_loop *loop,void *args)
{
	printf("======active task func=======\n");
	//得到当前thread在线的fd有哪些
	listen_fd_set fds;
	loop->get_listen_fds(fds);

	listen_fd_set::iterator it;

	//遍历
	for(it=fds.begin();it!=fds.end();it++){
		int fd=*it;//当前线程已经建立链接并且监听中当前在线客户端
		tcp_conn *conn=tcp_server::conns[fd];//取出链接
		if(conn!=NULL){
			int msgid=404;
			const char *msg="Hello I am a Task!";
			conn->send_message(msg,strlen(msg),msgid);
		}

	}
}
//定义一个回显业务
void callback_busi(const char *data,uint32_t len,int msgid,net_connection *conn,void *user_data)
{
	printf("callback busi is call..\n");
	conn->send_message(data,len,msgid);
	printf("conn param=%s\n",(char*)conn->param);
}

void print_busi(const char *data,uint32_t len,int msgid,net_connection *conn,void *user_data)
{
	printf("print busi is called\n");
	printf("recv from client :[%s]\n",data);
	printf("msgid:%d\n",msgid);
	printf("len=%d\n",len);
	
}

//新的客户端创建成功之后的回调
void on_client_build(net_connection *conn,void *args){
	int msgid=200;
	const char *msg="welcome ! you are online!!!";
	conn->send_message(msg,strlen(msg),msgid);

	//每次客户端在创建链接成功之后
	server->get_thread_pool()->send_task(print_lars_task);

	//给当前的conn绑定一个自定义参数，供之后来使用
	const char *conn_param_test="I am the conn param for you!";
	conn->param=(void*)conn_param_test;
}

//客户端断开之前的回调
void on_client_lost(net_connection *conn,void *args){
	printf("connection is lost\n");
}
int main()
{
	event_loop loop;

	//加载配置文件
	config_file::setPath("./reactor.ini");
	std::string ip = config_file::instance()->GetString("reactor","ip","0.0.0.0");
	short port = config_file::instance()->GetNumber("reactor","port",8888);

	server= new tcp_server(&loop, ip.c_str(),port);
	
	
	//注册一些回调方法
	server->add_msg_router(1,callback_busi);
	server->add_msg_router(2,print_busi);
	//注册连接hook函数
	server->set_conn_start(on_client_build);
	server->set_conn_close(on_client_lost);


	loop.event_process();

	return 0;
}

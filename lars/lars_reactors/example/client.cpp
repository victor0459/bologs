#include"tcp_client.h"
#include<string.h>

void callback_busi(const char *data,uint32_t len,int msgid,net_connection *conn,void *user_data)
{
	printf("callback busi is call..\n");
	conn->send_message(data,len,msgid);

}

void print_busi(const char *data,uint32_t len,int msgid,net_connection *conn,void *user_data)
{
	printf("print busi is called\n");
	printf("recv from client :[%s]\n",data);
	printf("msgid:%d\n",msgid);
	printf("len=%d\n",len);

}
//客户端创建连接之后的HOOK业务
void on_client_build(net_connection *conn,void *args)
{
	printf("-->on client build \n");
	//连接创建成功之后主动发包
	int msgid=1;
	const char *msg="hello lars";
	conn->send_message(msg,strlen(msg),msgid);
}
//客户端销毁连接之前的HOOk业务
void on_client_lost(net_connection *conn,void *args){
	printf("-->on client lost\n");
}

int main(){

	event_loop loop;
	tcp_client *client=new tcp_client(&loop,"127.0.0.1",7777);

	client->add_msg_router(1,print_busi);
	client->add_msg_router(2,callback_busi);
	client->add_msg_router(200,print_busi);
	client->add_msg_router(404,print_busi);

	//注册hook连接函数
	client->set_conn_start(on_client_build);
	client->set_conn_close(on_client_lost);
	loop.event_process();
	return 0;

}

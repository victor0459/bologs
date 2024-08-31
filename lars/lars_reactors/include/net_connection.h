#pragma once

//抽象的连接类d


class net_connection
{
	public:
	net_connection(){
	}

	//纯虚函数
	virtual int send_message(const char *data,int msglen,int msgid)=0;
	virtual int get_fd()=0;
	void *param;//开发者可以通过参数传递一些动态的自定义参数

};
typedef void (*conn_callback)(net_connection *conn,void *args);

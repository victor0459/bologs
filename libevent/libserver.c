#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<fcntl.h>
#include<event2/event.h>
#include<event2/bufferevent.h>
#include<event2/listener.h>
int main(int argc,const char*argv[]){
	//创建事件
	struct event_base*base=event_base_new();
	//创建监听的套接字
	
	//绑定
	
	//监听
	
	//等待并接受连接请求
	struct evconnlistener*listen=NULL;
	listen=evconnlistener_new_bind(base,listen_cb,NULL,LEV_OPT_CLOSE_FREE|LEV_OPT_REUSEABLE,-1,(struct sockaddr*)&serv,sizeof(serv));
	return 0;
}

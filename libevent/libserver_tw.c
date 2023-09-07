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
//连接完成之后对应的通信操作
void listen_cb(struct evconnlistener*listener,
		evutil_socket_t fd,
		struct sockaddr*addr,
		int len,void*ptr){
	struct event_base*base=(struct event_base*)ptr;
	//接受数据 发送数据
	//将fd封装一下 为bufferevent
	struct bufferevent*bev=NULL;
	bev=bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
	//给缓冲区对象的读写设置回调函数
	bufferevent_setcb(bev,read_cb,write_cb,event_cb,NULL);
}
int main(int argc,const char*argv[]){
	//创建事件
	struct event_base*base=event_base_new();
	//init server info
	struct sockaddr_in serv;
	memset(&serv,0,sizeof(serv));
	serv.sin_family=AF_INET;
	serv.sin_port=htons(8765);
	serv.sin_addr.s_addr=htonl(INADDR_ANY);
	//创建监听的套接字
	socket();		
	//绑定
	bind();
	//监听
	listen();
	//等待并接受连接请求
	struct evconnlistener*listen=NULL;
	listen=evconnlistener_new_bind(base,listen_cb,base,LEV_OPT_CLOSE_FREE|LEV_OPT_REUSEABLE,-1,(struct sockaddr*)&serv,sizeof(serv));
	event_base_dispatch(base);
	evconnlistener_free(listen);
	event_base_free(base);
	return 0;
}

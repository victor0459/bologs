#pragma once
#include<cstddef>
class event_loop;
typedef void io_callback(event_loop *loop,int fd,void *args);//回调格式

struct io_event{
	io_event(){
		mask=0;
		read_callback = NULL;
		write_callback = NULL;
		rcb_args = NULL;
		wcb_args = NULL;

	}
	//事件的读写属性EPOLLIN EPOLLOUT
	int mask;

	//读事件所触发的回调函数
	io_callback *read_callback;
	//写事件所触发的回调函数
	io_callback *write_callback;

	//读回调的参数
	void *rcb_args;
	//写回调的参数
	void *wcb_args;
};

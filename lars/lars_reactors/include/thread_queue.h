#pragma once
#include<queue>
#include<pthread.h>
#include<stdio.h>
#include"event_loop.h"
#include<sys/eventfd.h>
#include<unistd.h>
template <typename T>

class thread_queue{
	public:
	thread_queue(){
		_loop=NULL;
		pthread_mutex_init(&_queue_mutex,NULL);//非全局可用此
		//创建一个fd用来被监听的  能够被epoll监听的fd 没有与磁盘相关联 没有与socket相关联
		_evfd=eventfd(0,EFD_NONBLOCK);
		if(_evfd==-1){
			perror("eventfd()");
			exit(1);
		}		
	}
	~thread_queue(){
		pthread_mutex_destroy(&_queue_mutex);//销毁锁
		close(_evfd);
	}

	//向队列中添加一个任务 main_thread called or other called
	void send(const T &task){
		//将task加入到queue中，激活_evfd
		pthread_mutex_lock(&_queue_mutex);
		_queue.push(task);
		//激活_evfd可读事件，向_evfd写数据
		unsigned long long idle_num=1;
		int ret=write(_evfd,&idle_num,sizeof(unsigned long long));
		if(ret==-1){
			perror("evfd write error");
			exit(1);
		}
		pthread_mutex_unlock(&_queue_mutex);
	}
	
	//从队列中取出一个数据 将整个queue返回给上层 queue_msg传出型参数，被_evfd激活的读事件业务函数来调用
	void recv(std::queue<T>& queue_msgs){
		unsigned long long idle_num;
		//拿出queue数据
		pthread_mutex_lock(&_queue_mutex);
		//evfd所绑定事件读写缓存清空，将占位激活符号取出来
		int ret=read(_evfd,&idle_num,sizeof(unsigned long long));
		if(ret==-1){
			perror("evfd read error");
		}
		//交换两个容器的指针，保证queue_msgs是一个空队列，这样交换完成后 ，_queue才是以恶搞空队列
		std::swap(queue_msgs,_queue);
		pthread_mutex_unlock(&_queue_mutex);
	}
	//让当前的thread_queue被那个event_loop所监听
	void set_loop(event_loop *loop){
		this->_loop=loop;
	}
	
	//给当前对应设置一个_evfd如果触发EPOLLON事件所被激活的回调函数
	void set_callabck(io_callback *cb,void *args=NULL){
		if(_loop!=NULL){
			_loop->add_io_event(_evfd,cb,EPOLLIN,args);
		}
	}

	private:
	int _evfd;//让某个线程进行监听
	event_loop *_loop;//目前被那个loop所监听
	std::queue<T> _queue;//队列
	pthread_mutex_t _queue_mutex;//保护queue的互斥锁	


};

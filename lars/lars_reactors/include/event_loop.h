#pragma once

#include"event_base.h"
#include<ext/hash_map>
#include<ext/hash_set>
#include<sys/epoll.h>
#include<vector>
#define MAXEVENTS 10
typedef __gnu_cxx::hash_map<int,io_event> io_event_map;
typedef __gnu_cxx::hash_map<int,io_event>::iterator io_event_map_it;//迭代器
typedef __gnu_cxx::hash_set<int> listen_fd_set;

//定义一个task 一般任务的回调函数类型
typedef void (*task_func)(event_loop *loop,void *args);
class event_loop{
	public:
		event_loop();

		//阻塞循环监听事件
		void event_process();

		//添加一个io事件到event_loop中
		void add_io_event(int fd,io_callback *proc,int mask,void *args);
		//删除一个事件从event_loop中
		void del_io_event(int fd);

		//删除一个事件的某触发条件
		void del_io_event(int fd,int mask);
		//---->针对 异步task任务的方法
		//添加一个task任务到_ready_task集合中
		void add_task(task_func func,void *args);

		//执行全部ready_tasks里面的任务
		void execute_ready_tasks();
		//获取当前event_loop正在监听的fd集合
		void get_listen_fds(listen_fd_set &fds){
			fds=listen_fds;
		}
	private:
		int _epfd;

		io_event_map _io_evs;

		listen_fd_set listen_fds;

		struct epoll_event _fired_evs[MAXEVENTS];


		//--------->针对task属性
		typedef std::pair<task_func,void*> task_func_pair;
		//目前已经就绪的全部的task任务
		std::vector<task_func_pair> _ready_tasks;	
};

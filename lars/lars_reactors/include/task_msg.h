#pragma once

//thread_queue消息队列 所能接收的消息类型 新连接和新消息
struct task_msg{
	enum TASK_TYPE{
		NEW_CONN,
		NEW_TASK
	};

	TASK_TYPE type;

	//任务本身数据内容
	union{
		//如果是任务1 task_msg的任务内容就是一个刚刚创建好的connfd
		int connfd;

		//暂时用不上
		//如果是任务2 task_msg的任务应该由具体的数据参数和具体的回调业务
		struct{
			void (*task_cb)(event_loop *loop,void *args);
			void *args;
		};
		

	};

};

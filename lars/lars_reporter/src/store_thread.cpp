#include "store_report.h"
#include "lars.pb.h"
#include "lars_reactor.h"
#include<queue>
struct Args{
	thread_queue<lars::ReportStatusRequest>* queue;
	StoreReport *store_conn;
};

//从消息队列中取出上报数据进行入库
void thread_report(event_loop *loop,int fd,void *args){
	//消息队列已经有数据 需从队列中取出数据 进行入库
	thread_queue<lars::ReportStatusRequest> *queue=((Args*)args)->queue;
	StoreReport *store_conn=((Args*)args)->store_conn;

	std::queue<lars::ReportStatusRequest> report_msgs;
	queue->recv(report_msgs);
	while(!report_msgs.empty()){
		lars::ReportStatusRequest msg=report_msgs.front();
		report_msgs.pop();

		//StoreReport sr;
		store_conn->store(msg);
	}
	
}

//每个存储线程的主业务
void *store_main(void *args){
	thread_queue<lars::ReportStatusRequest> *queue=(thread_queue<lars::ReportStatusRequest>*)args;

	event_loop loop;

	//定义一个存储对象 每个线程创建一个长连接存储对象
	StoreReport sr;
	Args callback_args;
	callback_args.queue=queue;
	callback_args.store_conn=&sr;

	//此时需要将sr也传入 但是已经有一个结构体了 定义一个结构体打包
	queue->set_loop(&loop);
	queue->set_callabck(thread_report,&callback_args);
	
	loop.event_process();
	return NULL;	
}

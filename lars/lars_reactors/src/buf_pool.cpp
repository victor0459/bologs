#include"buf_pool.h"
#include<assert.h>

//单例对象
buf_pool*buf_pool::_instance=NULL;

//用于保证创建单例的init方法只执行一次的锁
pthread_once_t buf_pool::_once=PTHREAD_ONCE_INIT;

//用户保护内存池链表修改的互斥锁
pthread_mutex_t buf_pool::_mutex=PTHREAD_MUTEX_INITIALIZER;
void buf_pool::make_io_buf_list(int cap,int num)
{
	//1链表的头指针
	io_buf*prev;
	//2开辟内存池
	_pool[cap]=new io_buf(cap);
	if(_pool[cap]==NULL){
		fprintf(stderr,"new io_buf error\n");
		exit(1);
	}
	prev=_pool[cap];
	for(int i=1;i<num;i++){
		prev->next=new io_buf(cap);
		if(prev->next==NULL){
			fprintf(stderr,"new io_buf cap error\n");
			exit(1);
		}
		prev=prev->next;
	}
	_total_mem+=cap/1024*num;

}
buf_pool::buf_pool():_total_mem(0)
{
	make_io_buf_list(m4k,5000);
	make_io_buf_list(m16k,1000);
	make_io_buf_list(m64k,500);
	make_io_buf_list(m256k,200);
	make_io_buf_list(m1M,50);
	make_io_buf_list(m4M,20);
	make_io_buf_list(m8M,10);
}
//从内存池中申请一块内存
io_buf *buf_pool::alloc_buf(int N)
{
	int index;
	//1找到最接近的一个链表的刻度 返回一个io_buf
	if(N<=m4k){
		index=m4k;
	}else if(N<=m16k){
		index=m16k;
	}else if(N<=m64k){
		index=m64k;
	}else if(N<=m256k){
		index=m256k;
	}else if(N<=m1M){
		index=m1M;
	}else if(N<=m4M){
		index=m4M;
	}else if(N<=m8M){
		index=m8M;
	}else{
		return NULL;
	}
	//2如果该index内存没有了 需要重新申请
	pthread_mutex_lock(&_mutex);
	if(_pool[index]==NULL){
		//链表需要重新申请
		if(_total_mem+index/1024>=MEM_LIMIT){
			fprintf(stderr,"already use too many memory!\n");
			exit(1);
		}
		io_buf*new_buf=new io_buf(index);
		if(new_buf==NULL){
			fprintf(stderr,"new io_buf error\n");
			exit(1);
		}
		_total_mem+=index/1024;
		pthread_mutex_unlock(&_mutex);
		return new_buf;
	}
	io_buf*target=_pool[index];
	_pool[index]=target->next;
	pthread_mutex_unlock(&_mutex);
	target->next=NULL;
	return target;
}
io_buf *buf_pool::alloc_buf()
{

	return alloc_buf(m4k);
}
//重置一个io_buf放回pool中
void buf_pool::revert(io_buf*buffer)
{
	//将buffer放回到pool中
	//index属于pool中哪个链表
	int index=buffer->capacity;
	buffer->length=0;
	buffer->head=0;
	pthread_mutex_lock(&_mutex);
	//断言一定能找到index的key
	assert(_pool.find(index)!=_pool.end());
	//将buffer设置为对应的buf链表首节点
	buffer->next=_pool[index];
	_pool[index]=buffer;
	pthread_mutex_unlock(&_mutex);




}

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
void* thr(void*arg){
	printf("i am a thread!pid=%d,tid=%lu\n",getpid(),pthread_self());
	sleep(3);
	printf("i am a thread!pid=%d,tid=%lu\n",getpid(),pthread_self());
	return (void*)100;
}
int main(){
	pthread_t tid;
	pthread_create(&tid,NULL,thr,NULL);
	pthread_detach(tid);
	sleep(5);
	int ret=0;
	//线程分离
	if((ret=pthread_join(tid,NULL))>0){
		printf("join err:%d,%s\n",ret,strerror(ret));
	}
	return 0;
}

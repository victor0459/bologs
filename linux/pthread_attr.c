#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
void* thr(void*arg){
	printf("i am a thread!pid=%d,tid=%lu\n",getpid(),pthread_self());
	return NULL;
}
int main(){
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	//线程分离
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	pthread_t tid;
	pthread_create(&tid,&attr,thr,NULL);
	int ret=0;
	if((ret=pthread_join(tid,NULL))>0){
		printf("join err:%d,%s\n",ret,strerror(ret));
	}
	pthread_attr_destroy(&attr);
	return 0;
}

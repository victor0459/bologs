#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
void* thr(void*arg){
	printf("i am a thread!pid=%d,tid=%lu\n",getpid(),pthread_self());
	sleep(3);
	printf("i am a thread!pid=%d,tid=%lu\n",getpid(),pthread_self());
	return (void*)100;
}
int main(){
	pthread_t tid;
	pthread_create(&tid,NULL,thr,NULL);
	//printf("i am main  thread!pid=%d,tid=%lu\n",getpid(),pthread_self());
	//sleep(1);
	void *ret;
	pthread_join(tid,&ret);
	printf("ret exit with %d\n",(int)ret);
	pthread_exit(NULL);
	return 0;
}

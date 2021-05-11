#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
typedef struct ProdInfo{
	int num;
	struct ProdInfo *next;
	
}prodInfo;
prodInfo *head=NULL;
int beginnum=1000;
void*thr_producter(void*arg){
	//负责在链表添加数据
	while(1){
		prodInfo*prod=malloc(sizeof(prodInfo));
		prod->num=beginnum++;
		printf("---%s---self=%lu---%d\n",__FUNCTION__,pthread_self(),prod->num);
		pthread_mutex_lock(&mutex);
		prod->next=head;
		head=prod;
		pthread_mutex_unlock(&mutex);
		//发起通知 已生产好
		pthread_cond_signal(&cond);
		sleep(rand()%4);
	}
	return NULL;
}
void*thr_customer(void*arg){
	prodInfo*prod=NULL;
	while(1){
		//取数据
		pthread_mutex_lock(&mutex);
		if(head==NULL){
			pthread_cond_wait(&cond,&mutex);
		}
		prod=head;
		head=head->next;
		printf("---%s---self=%lu---%d\n",__FUNCTION__,pthread_self(),prod->num);
		pthread_mutex_unlock(&mutex);
		sleep(rand()%4);
		free(prod);
	}
	return NULL;
}
int main(){
	
	pthread_t tid[2];
	pthread_create(&tid[0],NULL,thr_producter,NULL);
	pthread_create(&tid[0],NULL,thr_customer,NULL);
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	return 0;
}

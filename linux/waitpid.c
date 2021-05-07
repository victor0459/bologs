#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>


int main(){
	pid_t pid =fork();
	if(pid==0){
		printf("i am child pid=%d\n",getpid());
		sleep(2);
	}
	else if(pid>0){
		printf(" parent pid=%d\n",getpid());
		int ret;
		while((ret=waitpid(-1,NULL,WNOHANG))==0){
			sleep(1);
		}
		printf("ret=%d\n",ret);
		while(1){
			sleep(1);
		}
	}
	return 0;

}

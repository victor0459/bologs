#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>



int main(){
	
	printf("begin...\n");
	pid_t pid=fork();
	if(pid<0){
		perror("fork err");
		exit(1);
	}
	if(pid==0){
		printf("child ,pid=%d,ppid=%d\n",getpid(),getppid());
		
	}else if(pid>0){
		printf("child pid=%d,self=%d,ppid=%d\n",pid,getpid(),getppid());
		sleep(10);
	}
	printf("end...\n");
	return 0;
}

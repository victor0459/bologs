#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main(){
	pid_t pid=0;
	for(int i=0;i<5;i++){
		pid=fork();
		if(pid==0){
			printf("child pid=%d,ppid=%d\n",getpid(),getppid());
			break;
		}else if(pid>0){
			printf("father pid=%d,ppid=%d\n",getpid(),getppid());

		}
	}
	while(1){
		slepp(1);
	}
	return 0;
}

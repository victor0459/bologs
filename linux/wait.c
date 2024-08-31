#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(){
	pid_t pid=fork();
	if(pid==0){
		printf("i am child will die\n");
		sleep(2);
		return 101;
	}else if(pid>0){
		printf("i am parent\n");
		int status;
		//父进程回收
		pid_t wpid=wait(&status);
		printf("wait ok,wpid=%d,pid=%d\n",wpid,pid);
		if(WIFEXITED(status)){
			printf("child exit with %d\n",WEXITSTATUS(status));
		}
		if(WIFSIGNALED(status)){
			printf("child killed by %d\n",WTERMSIG(status));
		}
		while(1){
			sleep(1);
		}
	}
	return 0;


}

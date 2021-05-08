#include<stdio.h>
#include<unistd.h>

int main(){
	int fd[2];
	pipe(fd);
	pid_t pid=fork();
	if(pid==0){
		write(fd[1],"hello",5);
	
	}else if(pid>0){
		char buf[12]={0};
		int ret=read(fd[0],buf,sizeof(buf));
		if(ret>0){
			write(STDOUT_FILENO,buf,ret);
		}
	}

	return 0;
}

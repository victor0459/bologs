#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<string.h>
#include<sys/wait.h>

int main(){
	//匿名映射这两个宏 unix没有	
	int *mem=mmap(NULL,4,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANON,-1,0);
	if(mem==MAP_FAILED){
		perror("mmap err");
		return -1;
	}
	pid_t pid=fork();
	if(pid==0){
		*mem=101;
		printf("child ,*mem=%d\n",*mem);
		sleep(3);	
		printf("child ,*mem=%d\n",*mem);
	}else if(pid>0){
		sleep(1);	
		printf("parent ,*mem=%d\n",*mem);
		*mem=1001;
		printf("parent ,*mem=%d\n",*mem);
		wait(NULL);
	}
	
	munmap(mem,4);
	return 0;
}

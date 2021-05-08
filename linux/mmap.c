#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<string.h>
int main(){
	int fd=open("mem.txt",O_RDWR);
	//创建映射区
	char*mem=mmap(NULL,8,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(mem==MAP_FAILED){
		perror("mem err");
		return -1;
	}
	//拷贝数据
	strcpy(mem,"hello");
	
	//释放
	munmap(mem,8);
	close(fd);
	return 0;
}

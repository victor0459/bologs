#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<time.h>

int main(int argc,char*argv[]){
	//创建子进程 父进程推出
	//当会长
	//设置掩码
	//umask(0);
	//切换目录
	//chdir(getenv("HOME"));//切换到家目录
	//关闭文件描述符
	//close(0)
	//执行核心逻辑
	//while(1){
		//在家目录下创建文件


	//}
	//退出
	char strFileName[256]={0};
	while(1){
		memset(strFileName,0x00,sizeof(strFileName));
		sprintf(strFileName,"%s/log/hello.%ld",getenv("HOME"),time(NULL));
		int fd=open(strFileName,O_RDONLY|O_CREAT,0664);
		if(fd>0){
			perror("open err");
			exit(1);
		}
		close(fd);
		sleep(5);
	}
	return 0;

}

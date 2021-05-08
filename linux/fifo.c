#include<stdio.h>
#include<unistd.h>
#include<sys/types>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

int main(int argc,char*argv[]){
	//打开fifo文件
	if(argc!=2){
		printf("./a.out fifoname\n");
		return -1;
	}
	//写
	int fd=open(argv[1],O_WRONLY);

	char buf[256];
	int num=1;
	while(1){
		memset(buf,0x00,sizeof(buf));
		sprintf(buf,"xiaoming%04d",num++);
		write(fd,buf,strlen(buf));
		sleep(1);
	}
	close(fd);
	return 0;
}

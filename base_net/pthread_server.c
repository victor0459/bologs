#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<ctype.h>
#include<errno.h>
#include<stdlib.h>
typedef struct SockInfo{
	int fd;
	struct sockaddr_in addr;
	pthread_t id;
	
}SockInfo;
void* worker(void*arg){
	SockInfo*info=(struct SockInfo*)arg;
	char buf[1024];
	char ip[64];
	while(1){
		printf("client_IP:%s,port:%d\n",inet_ntop(AF_INET,&info->addr.sin_addr.s_addr,ip,sizeof(ip)),ntohs(info->addr.sin_port));
		int len=read(info->fd,buf,sizeof(buf));
		if(len==-1){
			perror("read error");
			pthread_exit(NULL);
		}else if(len==0){
			close(info->fd);
			break;
		}else{
			printf("recv buf:%s\n",buf);
			write(info->fd,buf,len);
		}
	}
	
}

int main(int argc,const char*argv[]){
	if(argc<2){
		printf("eg: ./a.out port\n");
		exit(1);
	}
	struct sockaddr_in serv_addr;
	socklen_t serv_len=sizeof(serv_addr);
	int port=atoi(argv[1]);
	int lfd=socket(AF_INET,SOCK_STREAM,0);
	memset(&serv_addr,0,serv_len);
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(port);
	bind(lfd,(struct sockaddr*)&serv_addr,serv_len);
	listen(lfd,36);

	int i=0;

	SockInfo info[256];
	for(i=0;i<sizeof(info)/sizeof(info[0]);++i){
		info[i].fd=-1;

	}
	socklen_t cli_len=sizeof(struct sockaddr_in);
	while(1){
		//选择最小的文件描述符
		for(i=0;i<256;++i){
			if(info[i].fd==-1){
				break;
			}
		}
		if(i==256){
			break;
		}
		info[i].fd=accept(lfd,(struct sockaddr*)&info[i].addr,&cli_len);
		pthread_create(&info[i].id,NULL,worker,&info[i]);
		pthread_detach(info[i].id);
	}
	return 0;
}

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<ctype.h>
#include<signal.h>
#include<sys/wait.h>
#include<errno.h>
#include<sys/epoll.h>

int main(int argc,const char*argv[]){
	if(argc<2){
		printf("eg: ./a.out port\n");
		exit(1);
	}
	struct sockaddr_in serv_addr;
	socklen_t serv_len=sizeof(serv_addr);
	int port=atoi(argv[1]);
	//创建套接字
	int lfd=socket(AF_INET,SOCK_STREAM,0);
	//初始化服务器 sockaddr_in 清零
	memset(&serv_addr,0,serv_len);
	serv_addr.sin_family=AF_INET;//地址族
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);//监听本机所有IP
	serv_addr.sin_port=htons(port);//设置端口
	//绑定ip和端口
	bind(lfd,(struct sockaddr*)&serv_addr,serv_len);
	//设置同时监听的最大个数
	listen(lfd,36);
	printf("Start accept...\n");
	struct sockaddr_in client_addr;
	socklen_t cli_len=sizeof(client_addr);
	
	int epfd=epoll_create(2000);
	struct epoll_event ev;
	ev.events=EPOLLIN;
	ev.data.fd=lfd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&ev);
	struct epoll_event all[2000];
	while(1){
		//使用epoll同志内核fd 文件IO检测
		int ret=epoll_wait(epfd,all,sizeof(all)/sizeof(all[0]),-1);
		for(int i=0;i<ret;i++){
			int fd=all[i].data.fd;
			if(fd==lfd){
				int cfd=accept(lfd,(struct sockaddr*)&client_addr,&cli_len);
				if(cfd==-1){
					perror("accept error");
					exit(1);
				}
				struct epoll_event temp;
				temp.events=EPOLLIN;
				epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&temp);
			}else{
				if(!all[i].events&EPOLLIN){
					continue;
				}
				char buf[1024]={0};
				int len=recv(fd,buf,sizeof(buf),0);
				if(len==-1){
					perror("recv error");
					exit(1);
				}else if(len==0){
					printf("client disconnect...\n");
					close(fd);
					epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
				}else{
					printf("recv buf:%s\n",buf);
					write(fd,buf,len);
				}
			}
		}
	}

	close(lfd);
	return 0;
}

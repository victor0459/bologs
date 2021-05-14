#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<ctype.h>
#include<string.h>
#include<fcntl.h>
int main(int argc,const char*argv[]){
	if(argc<2){
		printf("eg: ./a.out port");
		exit(1);
	}	
	int port=atoi(argv[1]);
	struct sockaddr_in serv_addr;
	socklen_t serv_len=sizeof(serv_addr);
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(port);
	int lfd=socket(AF_INET,SOCK_STREAM,0);
	bind(lfd,(struct sockaddr*)&serv_addr,serv_len);
	listen(lfd,36);
	struct sockaddr_in client_addr;
	socklen_t cli_len=sizeof(client_addr);
	int epfd=epoll_create(2000);
	struct epoll_event ev;
	ev.events=EPOLLIN;
	ev.data.fd=lfd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&ev);
	struct epoll_event all[2000];
	while(1){
		int ret=epoll_wait(epfd,all,sizeof(all)/sizeof(all[0]),-1);
		for(int i=0;i<ret;i++){
			int fd=all[i].data.fd;
			if(lfd==fd){
				int cfd=accept(lfd,(struct sockaddr*)&client_addr,&cli_len);
				if(cfd==-1){
					perror("accept error");
					exit(1);
				}
				//设置文件cfd为非阻塞模式
				int flag=fcntl(cfd,F_GETFL);
				flag|=O_NONBLOCK;
				fcntl(cfd,F_SETFL,flag);
				struct epoll_event temp;
				temp.events=EPOLLIN;
				temp.data.fd=cfd;
				epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&temp);
			}else{
				if(!all[i].events&EPOLLIN){
					continue;
				}
				char buf[1024]={0};
				int len;
				while((len=recv(fd,buf,sizeof(buf),0))>){
					write(STDOUT_FILENO,buf,len);
					send(fd,buf,len,0);
				}
				if(len==0){
					printf("8888888888888888888\n");
				}else if(len==-1){
					if(errno==EAGAIN){
						printf("缓冲区数据已经读完\n");
					}else{
						printf("recv error---\n");
						exit(1);
					}
	
				}
#if 0
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
#endif				
			}
		}
	}
	close(lfd);
	return 0;
}

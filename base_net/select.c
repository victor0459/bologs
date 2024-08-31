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
#include<sys/select.h>

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
	//初始化服务器 sockaddr_in
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
	//最大的文件描述符
	int maxfd=lfd;
	fd_set reads,temp;
	FD_ZERO(&reads);
	FD_SET(lfd,&reads);
	while(1){
		temp=reads;
		int ret=select(maxfd+1,&temp,NULL,NULL,NULL);
		if(ret==-1){
			perror("select error");
			exit(1);
		}
		//客户端发起了新连接
		if(FD_ISSET(lfd,&temp)){
			//接受连接请求 accept不阻塞 非阻塞
			int cfd=accept(lfd,(struct sockaddr*)&client_addr,&cli_len);
			if(cfd==-1){
				perror("accept error");
				exit(1);
			}
			//将cfd加入到待检测的读集合 下一次就能检测到
			FD_SET(cfd,&reads);
			//更新最大文件描述符
			maxfd=maxfd<cfd?cfd:maxfd;
		}
		//已经连接上的客户端 发送数据
		for(int i=lfd+1;i<maxfd+1;++i){
			if(FD_ISSET(i,&temp)){
				char buf[1024]={0};
				int len=recv(i,buf,sizeof(buf),0);
				if(len==-1){
					perror("recv error");
					exit(1);
				}else if(len==0){
					printf("客户端已经断开连接\n");
					close(i);
					//从读集合中 删除
					FD_CLR(i,&reads);
				}else{
					printf("recv buf:%s\n",buf);
					send(i,buf,strlen(buf),0);
				}
			}
		}
	}


	return 0;
}

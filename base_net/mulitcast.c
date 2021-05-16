#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#include<net/if.h>
int main(int argc,const char*argv[]){
	int fd=socket(AF_INET,SOCK_DGRAM,0);
	if(fd==-1){
		perror("socker error");
		exit(1);
	}
	struct sockaddr_in serv;
	memset(&serv,0,sizeof(serv));
	serv.sin_family=AF_INET;
	serv.sin_port=htons(8787);
	serv.sin_addr.s_addr=htonl(INADDR_ANY);
	int ret=bind(fd,(struct sockaddr*)&serv,sizeof(serv));
	if(ret==-1){
		perror("bind error");
		exit(1);
	}
	//初始化客户端地址
	struct sockaddr_in client;
	memset(&client,0,sizeof(client));
	client.sin_family=AF_INET;
	client.sin_port=htons(6767);
	//使用组播地址给客户端发送数据 
	inet_pton(AF_INET,"239.0.0.10",&client.sin_addr.s_addr);
	//给服务器开放组播权限
	struct ip_mreqn flag;
	inet_pton(AF_INET,"239.0.0.10",&flag.imr_multiaddr.s_addr);
	inet_pton(AF_INET,"0.0.0.0",&flag.imr_address.s_addr);
	flag.imr_ifindex=if_nametoindex("ens33");
	setsockopt(fd,IPPROTO_IP,IP_MULTICAST_IF,&flag,sizeof(flag));
	while(1){
		static int num=0;
		char buf[1024]={0};
		sprintf(buf,"hello,udp==%d\n",num++);
		int ret=sendto(fd,buf,strlen(buf)+1,0,(struct sockaddr*)&client,sizeof(client));
		if(ret==-1){
			perror("sendto error");
			break;
		}
		printf("server==send buf:%s\n",buf);
		sleep(1);
	}
	close(fd);
	return 0;
}

#include"udp_server.h"
//服务端处理客户端的读事件
void server_read_callback(event_loop *loop,int fd,void *args){ 
	udp_server *server=(udp_server*)args;
	//处理读业务
	server->do_read();
 }

udp_server::udp_server(event_loop *loop,const char *ip,uint16_t port){

	//1忽略信号
	if (signal(SIGHUP, SIG_IGN) == SIG_ERR)  {
		fprintf(stderr, "signal ignore SIGHUB\n");
	}

	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)  {
		fprintf(stderr, "signal ignore SIGHUB\n");
	}
	//2创建套接字
	_sockfd=socket(AF_INET,SOCK_DGRAM|SOCK_NONBLOCK|SOCK_CLOEXEC,IPPROTO_UDP);
	if(_sockfd==-1){
		perror("create udp server");
		exit(1);
	}

	//3初始化服务器地址
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	inet_aton(ip,&server_addr.sin_addr);
	server_addr.sin_port=htons(port);

	//4绑定端口
	if(bind(_sockfd,(const struct sockaddr*)&server_addr,sizeof(server_addr))<0){
		fprintf(stderr,"bind error\n");
		exit(1);
	}

	//5添加loop事件监控
	this->_loop=loop;

	//6清空客户端地址
	bzero(&_client_addr,sizeof(_client_addr));
	_client_addrlen=sizeof(_client_addr);

	//监控_sockfd读事件，如果可读，代表客户端有数据
	_loop->add_io_event(_sockfd,server_read_callback,EPOLLIN,this);
	printf("udp server is running ip=%s,port=%d\n",ip,port);

}


//读客户端数据
void udp_server::do_read(){
	while(true){
		int pck_len=recvfrom(_sockfd,_read_buf,sizeof(_read_buf),0,(struct sockaddr*)&_client_addr,&_client_addrlen);
		if(pck_len==-1){
			if(errno==EINTR){
				//中断错误
				continue;
			}
			else if(errno==EAGAIN){
				//非阻塞错误 当前套接字不可读 steven
				break;
			}
			else{
				perror("udp server recvfrom error");
				break;
			}
		}

		//数据已经读过来了
		msg_head head;
		memcpy(&head,_read_buf,MESSAGE_HEAD_LEN);
		if(head.msglen>MESSAGE_LENGTH_LIMIT || head.msglen<0 || head.msglen+MESSAGE_HEAD_LEN!=pck_len){
			//报文格式有问题 不完整 过大 过小...
			fprintf(stderr,"error message head format error\n");
			continue;
		}
		//此时已经得到了一个完整的Message 通过msgid来调用对应的回调业务
		_router.call(head.msgid,head.msglen,_read_buf+MESSAGE_HEAD_LEN,this);
	}	

}


//udp主动发送消息
int udp_server::send_message(const char *data,int msglen,int msgid){
	//udp server 主动个发包给客户端
	if(msglen>MESSAGE_LENGTH_LIMIT){
		fprintf(stderr,"too large message\n");
		return -1;
	}
	//知果一个message包
	msg_head head;
	head.msglen=msglen;
	head.msgid=msgid;
	//将head写到输出缓冲中
	memcpy(_write_buf,&head,MESSAGE_HEAD_LEN);

	//将data写到输出缓冲中
	memcpy(_write_buf+MESSAGE_HEAD_LEN,data,msglen);

	//通过_client_addr将报文发送给对方客户端
	int ret=sendto(_sockfd,_write_buf,msglen+MESSAGE_HEAD_LEN,0,(struct sockaddr*)&_client_addr,_client_addrlen);
	if(ret==-1){
		perror("sendto()..");
		return -1;
	}

	return ret;
}

//注册一个msgid和回调业务的路由关系
void udp_server::add_msg_router(int msgid,msg_callback *cb,void *user_data){
	_router.register_msg_router(msgid,cb,user_data);

}
udp_server::~udp_server(){
	_loop->del_io_event(_sockfd);
	close(_sockfd);
}



#include"tcp_client.h"
#include<strings.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include"message.h"
void read_callback(event_loop *loop,int fd,void *args)
{
	tcp_client *cli=(tcp_client*)args;
	cli->do_read();
}
void write_callback(event_loop *loop,int fd,void *args)
{
	tcp_client *cli=(tcp_client*)args;
	cli->do_write();
}

tcp_client::tcp_client(event_loop *loop,const char *ip,unsigned short port)
{
	_sockfd=-1;
	//_msg_callback=NULL;
	_loop=loop;

	_conn_start_cb=NULL;
	_conn_close_cb=NULL;
	_conn_start_cb_args=NULL;
	_conn_close_cb_args=NULL;

	//封装即将要连接的远程server端的ip地址
	bzero(&_server_addr,sizeof(_server_addr));
	_server_addr.sin_family=AF_INET;
	inet_aton(ip,&_server_addr.sin_addr);
	_server_addr.sin_port=htons(port);
	_addrlen=sizeof(_server_addr);

	//连接远程客户端
	this->do_connect();

}
int tcp_client::send_message(const char *data,int msglen,int msgid)
{

	//printf("send_message call\n");发送
	bool active_epollout=false;
	if(obuf.length()==0){
		active_epollout=true;
	}

	msg_head head;
	head.msgid=msgid;
	head.msglen=msglen;
	//1将消息头写到obuf中
	int ret=obuf.send_data((const char*)&head,MESSAGE_HEAD_LEN);
	if(ret!=0){
		fprintf(stderr,"send head error\n");
		return -1;
	}

	//2写消息体
	ret=obuf.send_data(data,head.msglen);
	if(ret!=0){
		//如果消息体写失败 需要将消息头弹出
		obuf.pop(MESSAGE_HEAD_LEN);
		return -1;
	}

	if(active_epollout==true){
		_loop->add_io_event(_sockfd,write_callback,EPOLLOUT,this);
	}
	return 0;
}
void tcp_client::do_read()
{
	//printf("do_read call..\n");
	int ret=ibuf.read_data(_sockfd);
	if(ret==-1){
		fprintf(stderr,"read data from socket\n");
		this->clean_conn();
		return ;
	}
	else if(ret==0){
		//对端正常关闭
		printf("perr server closed\n");
		this->clean_conn();
	}
	msg_head head;
	//2读数据
	while(ibuf.length()>=MESSAGE_HEAD_LEN){
		//先读头部 得到msgid msglen
		memcpy(&head,ibuf.data(),MESSAGE_HEAD_LEN);
		if(head.msglen>MESSAGE_LENGTH_LIMIT || head.msglen<0){
			fprintf(stderr,"data format error,too large or too small,need close\n");
			this->clean_conn();
			break;
		}
		//判断得到的消息体长度 是否和头部里的长度一致
		if(ibuf.length()<MESSAGE_HEAD_LEN+head.msglen){
			//缓存中buf剩余的数据 小于应该接收到的数据
			//说明当前不是一个完整的包
			break;
		}
		//表示当前的包是合法的
		ibuf.pop(MESSAGE_HEAD_LEN);

		//if(_msg_callback!=NULL){
			//this->_msg_callback(ibuf.data(),head.msglen,head.msgid,this,NULL);
		//}

		//调用注册好的回调业务
		this->_router.call(head.msgid,head.msglen,ibuf.data(),this);
		
		//整个消息处理wanle
		ibuf.pop(head.msglen);



	}
	ibuf.adjust();
	return ;
}

void tcp_client::do_write()
{
	while(obuf.length()){
		int write_num=obuf.write2fd(_sockfd);
		if(write_num==-1){
			fprintf(stderr,"tcp_client write sockfd error\n");
			this->clean_conn();
			return ;
		}
		else if(write_num==0){
			//当前不可写
			break;
		}
	}
	if(obuf.length()==0){
		//数据已经全部写完 将_sockfd事件删除
		_loop->del_io_event(_sockfd,EPOLLOUT);
	}	
	return ;
}

void tcp_client::clean_conn()
{
	//调用连接销毁之前的HOOk函数
	if(_conn_close_cb!=NULL){
		_conn_close_cb(this,_conn_close_cb_args);
	}


	if(_sockfd!=-1){
		printf("clean conn,del sock\n");
		_loop->del_io_event(_sockfd);
		close(_sockfd);
	}
}

void connection_succ(event_loop *loop,int fd,void *args)
{
	tcp_client *cli=(tcp_client*)args;
	loop->del_io_event(fd);
	//再次对当前fd进行一次判断 错误编码获取 如果没有任何错误 那么一定成功
	int result=0;
	socklen_t result_len=sizeof(result);
	getsockopt(fd,SOL_SOCKET,SO_ERROR,&result,&result_len);
	if(result==0){
		//fd中没有任何错误
		//连接创建成功
		printf("connection succ\n");

		//连接创建成功的之后的一些业务 客户端业务
		//主动发送一些消息给server端
		if(cli->_conn_start_cb!=NULL){
			cli->_conn_start_cb(cli,cli->_conn_start_cb_args);
		}


		//针对当前cli fd的读回调
		loop->add_io_event(fd,read_callback,EPOLLIN,cli);

		if(cli->obuf.length()!=0){
			//让event_loop处罚写回调函数
			loop->add_io_event(fd,write_callback,EPOLLOUT,cli);
		}

	}
	else{

		//连接创建失败
		fprintf(stderr,"connection %s:%d error\n",inet_ntoa(cli->_server_addr.sin_addr),ntohs(cli->_server_addr.sin_port));
		return ;

	}

}


void tcp_client::do_connect()
{
	if(_sockfd!=-1){
		close(_sockfd);
	}
	//创建套接字 非阻塞
	_sockfd=socket(AF_INET,SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK,IPPROTO_TCP);
	if(_sockfd==-1){
		fprintf(stderr,"create tcp client socket error\n");
		exit(1);
	}
	
	int ret=connect(_sockfd,(const struct sockaddr *)&_server_addr,_addrlen);
	if(ret==0){
		//创建连接成功
		printf("connect ret==0,connect%s:%d succ\n",inet_ntoa(_server_addr.sin_addr),ntohs(_server_addr.sin_port));

		//通过message发送消息给服务器
		//
		//监听读事件 绑定回调函数
		//_loop->add_io_event(_sockfd,read_callback,EPOLLIN,this);
		connection_succ(_loop,_sockfd,this);
	}
	else{
		if(errno==EINPROGRESS){
			//fd是非阻塞的 会出现这个错误 但并不是创建连接失败
			//如果fd是可写的 那么连接创建成功
			printf("do_connect,EINPROGRESS\n");
			//让event_loop去检测当前的sockfd是否可写 当前回调执行说明可写
			_loop->add_io_event(_sockfd,connection_succ,EPOLLOUT,this);

		}
		else{
			//连接失败
			fprintf(stderr,"connection error\n");
			exit(1);
		}
	}



}

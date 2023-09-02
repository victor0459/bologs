
#include"tcp_server.h"
#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<errno.h>
#include"reactor_buf.h"
#include"config_file.h"
//---------------------连接管理类---------------------
tcp_conn** tcp_server::conns;
pthread_mutex_t tcp_server::_conns_mutex=PTHREAD_MUTEX_INITIALIZER;
int tcp_server::_max_conns=0;
int tcp_server::_curr_conns=0;

//---------------------初始化 路由分发机制----------------------
msg_router tcp_server::router;

//=====================初始化hook函数=======================
conn_callback tcp_server::conn_start_cb=NULL;
conn_callback tcp_server::conn_close_cb=NULL;

void *tcp_server::conn_start_cb_args=NULL;
void *tcp_server::conn_close_cb_args=NULL;
//==========================================================


void tcp_server::increase_conn(int connfd,tcp_conn *conn)//新增一个连接
{
	pthread_mutex_lock(&_conns_mutex);
	conns[connfd]=conn;
	_curr_conns++;
	pthread_mutex_unlock(&_conns_mutex);
}
void tcp_server::decrease_conn(int connfd)
{
	pthread_mutex_lock(&_conns_mutex);
	conns[connfd]=NULL;
	_curr_conns--;
	pthread_mutex_unlock(&_conns_mutex);
}

void tcp_server::get_conn_num(int *curr_conn)
{
	pthread_mutex_lock(&_conns_mutex);

	*curr_conn=_curr_conns;

	pthread_mutex_unlock(&_conns_mutex);
}

//--------------------连接管理类end------------------------

void accept_callback(event_loop *loop,int fd,void *args)
{
	tcp_server *server=(tcp_server*)args;
	server->do_accept();
}
tcp_server::tcp_server(event_loop * loop,const char*ip,uint16_t port)
{
	bzero(&_connaddr,sizeof(_connaddr));
	//忽略一些信号SIGHUP SIGPIPE
	//SIGPIPE->如果客户端关闭 服务端再次write就会产生
	//SIGHUP->如果terminal关闭，会给当前进程发送该信号
	if(signal(SIGHUP,SIG_IGN)==SIG_ERR){
		fprintf(stderr,"signal ignore SIGHUP\n");

	}
	if(signal(SIGPIPE,SIG_IGN)==SIG_ERR){
		fprintf(stderr,"signal  ignore SIGPIPE\n");

	}

	//创建套接字
	_sockfd=socket(AF_INET,SOCK_STREAM|SOCK_CLOEXEC,IPPROTO_TCP);
	if(_sockfd==-1){
		fprintf(stderr,"tcp_server::socket()\n");
		exit(1);
	}
	//初始化地址
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	inet_aton(ip,&server_addr.sin_addr);
	server_addr.sin_port=htons(port);

	//可以多次监听 设置REUSE属性
	int op;
	if(setsockopt(_sockfd,SOL_SOCKET,SO_REUSEADDR,&op,sizeof(op))<0){
		fprintf(stderr,"setsock_opt err\n");
	}
	//绑定端口
	if(bind(_sockfd,(const struct sockaddr*)&server_addr,sizeof(server_addr))<0){
		fprintf(stderr,"bind error\n");
		exit(1);
	}
	//监听端口
	if(listen(_sockfd,500)==-1){
		fprintf(stderr,"listen error\n");
		exit(1);
	}
	//将形参loop添加到tcp_server _loop中
	_loop=loop;


	//7创建线程池 
	//int thread_cnt=3;//从配置文件中读取 建议配置的个数和用来网络通信的cpu个数一致
	int thread_cnt=config_file::instance()->GetNumber("reactor","threadNum",3);
	if (thread_cnt>0){
		_thread_pool=new thread_pool(thread_cnt);
		if (_thread_pool==NULL){
			fprintf(stderr,"tcp_server new thread_pool err\n");
			exit(1);
		}
	}
	//创建链接管理类
	_max_conns=config_file::instance()->GetNumber("reactor","maxConn",1000);//TODO 从配置文件中读取
	conns=new tcp_conn* [_max_conns+5+2*thread_cnt];
	if(conns==NULL){
		fprintf(stderr,"new conns[%d] error\n",_max_conns);
		exit(1);
	}
	//注册sockfd的读事件--->accept处理
	_loop->add_io_event(_sockfd,accept_callback,EPOLLIN,this);


}
void tcp_server::do_accept()
{
	int connfd;
	while(true){
		_addrlen=1;
		printf("begin accept\n");
		connfd=accept(_sockfd,(struct sockaddr*)&_connaddr,&_addrlen);
		if(connfd==-1){
			if(errno==EINTR){
				fprintf(stderr,"accept=EINTR\n");
				continue;
			}else if(errno==EMFILE){
				//连接过多 资源不足
				fprintf(stderr,"accept errno=DEMFILE\n");
			}else if(errno==EAGAIN){
				fprintf(stderr,"accept errno==EAGAIN\n");
				break;
			}else{
				fprintf(stderr,"accept error");
				exit(1);
			}

		}else{
			//连接成功
			//TODO 添加心跳机制

			//判断连接个数是否超过最大值_max_conns
			int cur_conns;
			get_conn_num(&cur_conns);
			if(cur_conns >= _max_conns){
				fprintf(stderr,"so many connections ,max=%d\n",_max_conns);
				close(connfd);
			}
			else{	
				if(_thread_pool!=NULL){
					//开启了多线程模式
					//将connfd叫个一个线程去创建 并且监听
					//1从线程池中获取一个thread_queue
					thread_queue<task_msg> *queue=_thread_pool->get_thread();
					//创建一个任务
					task_msg task;
					task.type=task_msg::NEW_CONN;
					task.connfd=connfd;
					//2将connfd发送到thread_queue中
					queue->send(task);
				}
				else{
					//没有开启多线程模式
					//创建一个新的tcp_conn连接对象
					tcp_conn *conn=new tcp_conn(connfd,_loop);
					if(conn==NULL){
						fprintf(stderr,"new tcp_conn error\n");
						exit(1);
					}
				}
			}
			break;
		}
	}
}
tcp_server::~tcp_server()
{
	close(_sockfd);

}


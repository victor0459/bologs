#pragma once
#include<ext/hash_map>
#include"tcp_conn.h"

//解决tcp粘包问题
struct msg_head{
	int msgid;//消息的数据类型
	int msglen;//消息的长度
};


//消息的长度 固定值
#define MESSAGE_HEAD_LEN 8

//消息头+消息体 最大长度限制
#define MESSAGE_LENGTH_LIMIT (65535-MESSAGE_HEAD_LEN)
//回调格式 msg_callback
typedef void msg_callback(const char *data,uint32_t len,int msgid,net_connection* conn,void *user_data);

//定义一个路由消息分发机制
class msg_router{
	public:
	
		//msg构造函数
		msg_router():_router(),_args(){
		}
		//给一个消息注册一个对应的回调业务函数
		int register_msg_router(int msgid,msg_callback *msg_cb,void *user_data){
			if(_router.find(msgid)!=_router.end()){
				//该msgid的回调业务已经存在
				printf("msgId%d is already register...\n",msgid);
				return -1;
			}
			//printf("add msg callback msgid=%d\n",msgid);
			_router[msgid]=msg_cb;
			_args[msgid]=user_data;
			return 0;
		}

		//调用注册的回调业务
		 void call(int msgid,uint32_t msglen,const char *data,net_connection* conn){
			//printf("call msgid=%d\n",msgid);
			if(_router.find(msgid)==_router.end()){
				printf("msgid:%d is not register\n",msgid);
				return ;
			}

			//证明_router中有回调函数 取出来 并执行
			msg_callback *callback=_router[msgid];
			void *userdata=_args[msgid];
			//调用注册的回调函数
			callback(data,msglen,msgid,conn,userdata);
			//printf("=========================\n");
		 }


	private:
	__gnu_cxx::hash_map<int,msg_callback*> _router;
	__gnu_cxx::hash_map<int,void*> _args;

};

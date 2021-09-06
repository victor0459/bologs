#include "reactor_buf.h"
#include<sys/ioctl.h>
#include<string.h>

reactor_buf::reactor_buf()
{
	_buf=NULL;
}
reactor_buf::~reactor_buf()
{
	this->clear();

}
const int reactor_buf::length() const
{
	return _buf!=NULL?_buf->length:0;

}
void reactor_buf::pop(int len)
{
	assert(_buf!=NULL && len <= _buf->length);
	_buf->pop(len);
	//当此时_buf可用长度为0
	if(_buf->length==0){
		this->clear();
	}
}
void reactor_buf::clear()
{
	if(_buf!=NULL){
		//将_buf重新放回到buf_pool中
		buf_pool::instance()->revert(_buf);
		_buf=NULL;
	}

}

//-----------------------------------------------------------------
//从一个fd中读取数据到reactor_buf中
int input_buf::read_data(int fd)
{
	int need_read;//硬件中有多少数据是可以读的
	
	//一次性将io的缓存数据全部读出来
	//需要给fd设置一个属性
	//传出一个参数，目前缓冲区中一共有多少数据是可读的
	if(ioctl(fd,FIONREAD,&need_read)==-1){
		fprintf(stderr,"ioctl FIONREAD\n");
		return -1;
	}
	if(_buf==NULL){
		//如果当前的input_buf里的_buf是空，需要从buf_pool拿一个新的
		_buf=buf_pool::instance()->alloc_buf(need_read);
		if(_buf==NULL){
			fprintf(stderr,"no buf for alloc!\n");
			return -1;
		}
	}else{
		//如果当前buf可用，判断一下当前buf是否够存
		assert(_buf->head==0);
		if(_buf->capacity - _buf->length < need_read){
			//不够存 需要重新申请
			io_buf*new_buf=buf_pool::instance()->alloc_buf(need_read+_buf->length);
			if(new_buf==NULL){
				fprintf(stderr,"no buf for alloc\n");
				return -1;
			}
		
			//将之前的_buf数据拷贝到新的buf中
			new_buf->copy(_buf);
			//将之前的_buf放回到内存池中
			buf_pool::instance()->revert(_buf);
			//新申请的buf成为当前的io_buf
			_buf=new_buf;
		}
	}
	int already_read=0;
	do{
		if(need_read==0){
			already_read=read(fd,_buf->data+_buf->length,m4k);//阻塞直到有数据
		}else{
			already_read=read(fd,_buf->data+_buf->length,need_read);
		}
	}while(already_read==-1&&errno==EINTR);//systemcall一个终端 良性
	
	if(already_read>0){
		if(need_read!=0){
			assert(already_read==need_read);
		}
		//读数据成功
		_buf->length+=already_read;
	}
	return already_read;
}

const char* input_buf::data()
{
	return _buf!=NULL?_buf->data+_buf->head:NULL;
}
//重置缓冲区
void input_buf::adjust()
{
	if(_buf!=NULL){
		_buf->adjust();
	}
}

//-----------------------------------------------------------------------
int output_buf::send_data(const char*data,int datalen)
{
	//注意 此函数不是将数据写到缓冲区 而是开辟内存空间
	if(_buf==NULL){
		//如果io_buf为空，从内存池中申请
		_buf=buf_pool::instance()->alloc_buf(datalen);
		if(_buf==NULL){
			fprintf(stderr,"no idle buf for alloc\n");
			return -1;
		}
	}else{
		assert(_buf->head==0);
		if(_buf->capacity-_buf->length<datalen){
			//证明不够存 需要从内存池中申请
			io_buf*new_buf=buf_pool::instance()->alloc_buf(datalen+_buf->length);
			if(new_buf==NULL){
				fprintf(stderr,"no idle buf for alloc\n");
				return -1;
			}
			//将之前的数据拷贝到新申请的buf中
			new_buf->copy(_buf);
			//将之前的buf放回到内存池中
			buf_pool::instance()->revert(_buf);
			//新申请的buf成为当前io_buf
			_buf=new_buf;
		}
	}
	//将data数据拷贝到io_buf中，拼接到后面
	memcpy(_buf->data+_buf->length,data,datalen);
	_buf->length+=datalen;
	return 0;
	
}
int output_buf::write2fd(int fd)
{
	assert(_buf!=NULL&&_buf->head==0);
	int already_write=0;
	do {
		already_write=write(fd,_buf->data,_buf->length);
	}while(already_write==-1&&errno==EINTR);//systemcall 引起的系统中断 继续写

	if(already_write>0){
		//已经处理的数据清空
		_buf->pop(already_write);
		//未处理的数据前置 覆盖老数据
		_buf->adjust();
	}
	//如果fd非阻塞 可能会引起EAGAIN错误
	if(already_write==-1&&errno==EAGAIN){
		already_write=0;//不是错误仅仅返回0 表示目前不可继续写
	}
	return already_write;

}
















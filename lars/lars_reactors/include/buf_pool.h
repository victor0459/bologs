#pragma once
#include<ext/hash_map>
#include"io_buf.h"

typedef __gnu_cxx::hash_map<int,io_buf*> pool_t;

enum MEM_CAP{

	m4k	=4096,
	m16k	=16384,
	m64k	=65536,
	m256k	=262144,
	m1M	=1058576,
	m4M	=4194304,
	m8M	=8388608
};

//总内存池最大限制 单位为 kb 目前限制是5GB
#define MEM_LIMIT (5U*1024*1024)
class buf_pool
{
	public:
		//初始化单例对象
		static void init(){
			//创建单例
			_instance=new buf_pool();
		}
		//获取单例方法
		static buf_pool*instance(){
			//被整init方法在这个进程中 只执行一次
			pthread_once(&_once,init);
			return _instance;
		}
		//开辟一个io_buf
		io_buf*alloc_buf(int N);
		//no pargma
		io_buf*alloc_buf();

		//重置一个io_buf
		void revert(io_buf*buffer);
		 void make_io_buf_list(int cap,int num);
	private:
		buf_pool();

		//拷贝构造私有化
		buf_pool(const buf_pool&);
		//等号重载符私有化
		const buf_pool& operator=(const buf_pool&);
		//多有buffer的一个map集合句柄
		pool_t _pool;

		//总buffer池的内存大小 单位为kb
		uint64_t _total_mem;
		//单例对象
		static buf_pool* _instance;
		//用于保证创建单例的init方法只执行一次的锁
		static pthread_once_t _once;
		//用于保护内存池链表的互斥锁
		static pthread_mutex_t _mutex;




};

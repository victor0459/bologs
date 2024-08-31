#pragma once 
//定义一个buffer存储数据的结构
class io_buf{
	public:
		//1构造，创建一个io_buf类
		io_buf(int size);

		//清空数据
		void clear();

		//将已经处理的数据清空,将未处理的数据提前至数据首地址
		void adjust();

		//将其他buf中的数据拷贝到自己中
		void copy(const io_buf*other);

		//处理长度为len的数据，移动head和修正length
		void pop(int len);

		//如果存在多个buffer，采用链表的形式连接起来
		io_buf*next;


		//当前buffer的缓存容量大小
		int capacity;

		//当前buffer的有效数据长度
		int length;

		//未处理数据的头部索引
		int head;

		//当前io_buf 所保存的数据地址
		char*data;



};

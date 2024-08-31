#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#ifdef __cplusplus
extern "C" {
#endif

#define MAX 1024

	struct SeqStack {
		void* data[MAX];//存放数据的数组
		int size;//栈中元素的个数
	};
	typedef void* SeqStack;
	//栈初始化
	SeqStack Init_SeqStack();
	//入栈
	void push_SeqStack(SeqStack stack, void* data);
	//出栈
	void Pop_SeqStack(SeqStack stack);
	//获得栈顶元素
	void* Top_SeqStack(SeqStack stack);
	//获得栈的大小
	int Size_SeqStack(SeqStack stack);
	//销毁栈
	void Destory_SeqStack(SeqStack stack);



#ifdef __cplusplus
}
#endif

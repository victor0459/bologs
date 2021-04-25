#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//链表节点的数据类型
struct QueueNode {
	struct QueueNode* next;
};
//链表的数据类型
struct LQueue {
	struct QueueNode header;
	int size;
	struct QueueNode* rear;//尾指针  链表的尾部 最后一个节点
};
typedef void* LinkQueue;
#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

//初始化
LinkQueue Init_LinkQueue();
//入队
void Push_LinkQueue(LinkQueue queue, void* data);
//出队
void Pop_LinkQueue(LinkQueue queue);
//获得队头元素
void* Front_LinkQueue(LinkQueue queue);
//获得队尾元素
void* Back_LinkQueue(LinkQueue queue);
//大小
int Size_LinkQueue(LinkQueue queue);
//销毁队列
void Destory_LinkQueue(LinkQueue queue);
#ifdef __cplusplus
}
#endif // __cplusplus

#include"LinkQueue.h"
//初始化
LinkQueue Init_LinkQueue() {
	struct LQueue* lq = malloc(sizeof(struct LQueue));
	if (NULL == lq) {
		return NULL;
	}
	lq->header.next = NULL;
	lq->rear = &(lq->header);
	lq->size = 0;
	return lq;
}
//入队
void Push_LinkQueue(LinkQueue queue, void* data) {
	if (queue == NULL) {
		return;
	}
	if (data == NULL) {
		return;
	}
	struct LQueue* q = (struct LQueue*)queue;
	struct QueueNode* n = (struct QueueNode*)data;
	q->rear->next = n;
	n->next = NULL;
	q->rear = n;
	q->size++;
}
//出队
void Pop_LinkQueue(LinkQueue queue) {
	if (NULL == queue) {
		return;
	}
	struct LQueue* q = (struct LQueue*)queue;
	if (q->size == 0) {
		return;
	}
	if (q->size == 1) {
		q->header.next = NULL;
		q->rear = &(q->header);
		q->size--;
		return;
	}
	struct QueueNode* pFrist = q->header.next;
	q->header.next = pFrist->next;
	q->size--;
}
//获得队头元素
void* Front_LinkQueue(LinkQueue queue) {
	if (NULL == queue) {
		return;
	}
	struct LQueue* q = (struct LQueue*)queue;
	return q->header.next;
}
//获得队尾元素
void* Back_LinkQueue(LinkQueue queue) {
	if (NULL == queue) {
		return;
	}
	struct LQueue* q = (struct LQueue*)queue;
	return q->rear;
}
//大小
int Size_LinkQueue(LinkQueue queue) {
	if (NULL == queue) {
		return;
	}
	struct LQueue* q = (struct LQueue*)queue;
	return q->size;
}
//销毁队列
void Destory_LinkQueue(LinkQueue queue) {
	if (NULL == queue) {
		return;
	}
	free(queue);
	queue = NULL;

}

#pragma once
#include"LinkList.h"
//链表节点数据类型
struct LinkNode {
	void* data;
	struct LinkNode* next;
};
//链表数据类型
struct LList {
	struct LinkNode header;
	int size;
};
//初始化链表
LinkList Init() {
	struct LList* list = malloc(sizeof(struct LList));
	if (NULL == list) {
		return NULL;
	}
	list->header.data = NULL;
	list->header.next = NULL;
	list->size = 0;
	return list;
}
//插入节点
void Insert_LinkList(LinkList list, int pos, void* data) {
	if (NULL == list) {
		return;
	}
	if (NULL == data) {
		return;
	}
	struct LList* myList = (struct LList*)list;
	if (pos<0 || pos>myList->size) {
		pos = myList->size;
	}
	//查找插入位置
	struct LinkNode* pCurrent = &(myList->header);
	for (int i = 0; i < pos; i++) {
		pCurrent = pCurrent->next;
	}
	//新节点插入到链表中
	struct LinkNode* newnode = malloc(sizeof(struct LinkNode));
	newnode->data = data;
	newnode->next = NULL;
	newnode->next = pCurrent->next;
	pCurrent->next = newnode;
	myList->size++;
}
//遍历链表
void Foreach_LinkList(LinkList list, FOREACH myforeach) {
	if (NULL ==list) {
		return;
	}
	if (myforeach == NULL) {
		return;
	}
	struct LList* mylist = (struct LList*)list;
	struct LinkNode* pCurrent = mylist->header.next;
	while (pCurrent != NULL) {
		myforeach(pCurrent->data);
		pCurrent = pCurrent->next;
	}
}
//按位删除
void RemoveByPos_LinkList(LinkList list, int pos) {
	if (NULL == list) {
		return;
	}
	struct LList* mylist = (struct LList*)list;
	if (pos<0 || pos>mylist->size - 1) {
		return;
	}
	//找位置
	struct LinkNode* pCurrent = &(mylist->header);
	for (int i = 0; i < pos; i++) {
		pCurrent = pCurrent->next;
	}
	struct LinkNode* pDel = pCurrent->next;
	pCurrent->next = pDel->next;
	free(pDel);
	pDel = NULL;
	mylist->size--;
}
//按值删除
void RemoveByValue_LinkList(LinkList list, void* data, COMPARE compare) {
	if (NULL == list) {
		return;
	}
	if (data == NULL) {
		return;
	}
	if (NULL == compare) {
		return;
	}
	struct LList* mylist = (struct LList*)list;
	struct LinkNode* pCurrent = &(mylist->header);
	struct LinkNode* pPrev = pCurrent->next;
	while (pCurrent != NULL) {
		if (compare(pCurrent->data, data)) {
			//找到了
			pPrev->next = pCurrent->next;
			free(pCurrent);
			pCurrent = NULL;
			mylist->size--;
			break;
		}
		pPrev = pCurrent;
		pCurrent = pCurrent->next;
	}
	
}
//清空链表
void Clear_LinkList(LinkList list) {
	if (NULL == list) {
		return;
	}
	struct LList* mylist = (struct LList*)list;
	struct LinkNode* pCurrent = mylist->header.next;
	while (pCurrent != NULL) {
		//先缓存下一个节点的地址
		struct LinkNode* pNext = pCurrent->next;
		free(pCurrent);
		pCurrent = pNext;
	}
	mylist->header.next = NULL;
	mylist->size = 0;
}
//大小
int Size_LinkList(LinkList list) {
	if (NULL == list) {
		return;
	}
	struct LList* mylist = (struct LList*)list;
	return mylist->size;
}
//销毁链表
void Destory_LinkList(LinkList list) {
	if (NULL == list) {
		return;
	}
	struct LList* mylist = (struct LList*)list;
	struct LinkNode* pCurrent = mylist->header.next;
	while (pCurrent != NULL) {
		//先缓存下一个节点的地址
		struct LinkNode* pNext = pCurrent->next;
		free(pCurrent);
		pCurrent = pNext;
	}
	free(list);
	list = NULL;
	mylist->size = 0;
}

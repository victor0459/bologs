#include"SeqStack.h"
//栈初始化
SeqStack Init_SeqStack() {
	struct SeqStack* stack = malloc(sizeof(struct SeqStack));
	if (NULL == stack) {
		return NULL;
	}
	memset(stack, 0, sizeof(struct SeqStack));
	stack->size = 0;
	return stack;
}
//入栈
void push_SeqStack(SeqStack stack, void* data) {
	if (NULL == stack) {
		return;
	}
	if (data == NULL) {
		return;
	}
	struct SeqStack* s = (struct SeqStack*)stack;
	s->data[s->size] = data;
	s->size++;
}
//出栈
void Pop_SeqStack(SeqStack stack) {
	if (NULL == stack) {
		return;
	}
	struct SeqStack* s = (struct SeqStack*)stack;
	s->data[s->size-1] = NULL;
	s->size--;
}
//获得栈顶元素
void* Top_SeqStack(SeqStack stack) {
	if (NULL == stack) {
		return;
	}
	struct SeqStack* s = (struct SeqStack*)stack;
	if (s->size == 0) {
		return NULL;
	}
	return s->data[s->size - 1];
	
}
//获得栈的大小
int Size_SeqStack(SeqStack stack) {
	if (NULL == stack) {
		return;
	}
	struct SeqStack* s = (struct SeqStack*)stack;
	
	return s->size;
}
//销毁栈
void Destory_SeqStack(SeqStack stack) {
	if (NULL == stack) {
		return;
	}
	free(stack);
}

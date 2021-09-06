#include"io_buf.h"

#include<stdio.h>
#include<assert.h>
#include<string.h>

//构造函数
io_buf::io_buf(int size)
{
	capacity=size;
	length=0;
	head=0;
	data=new char[size];
	assert(data);
}
void io_buf::clear()
{
	length=head=0;
}
void io_buf::adjust()
{
	if(head!=0){
		if(length!=0){
			memmove(data,data+head,length);
		}
		head=0;
	}
}

void io_buf::copy(const io_buf*other)
{
	memcpy(data,other->data+other->head,other->length);
	head=0;
	length=other->length;
}
void io_buf::pop(int len)
{
	length-=len;
	head+=len;

}

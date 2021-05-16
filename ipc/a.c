#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
int main(int argc,const char*argv[]){
	
	int ret=unlink("b.c");
	if(ret==-1){
		printf("unlink error\n");
		return -1;
	}
	printf("unlink success\n");


	return 0;	
}

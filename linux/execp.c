#include<unistd.h>
#include<stdio.h>
int main(){
	execlp("ls","ls","-l",NULL);

	//不需要判断返回值
	perror("exec err");

	return 0;
}

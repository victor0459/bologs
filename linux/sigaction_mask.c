#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void catch_sig(int num){
	printf("begin call,catch %d sig\n",num);
	sleep(5);
	printf("end call ,catch %d sig\n",num);
}
int main(){
	struct sigaction act;
	act.sa_flags=0;
	sigemptyset(&act.sa_mask);
	//临时平比的信号
	sigaddset(&act.sa_mask,SIGQUIT);
	act.sa_handler=catch_sig;
	//注意捕捉
	sigaction(SIGINT,&act,NULL);
	while(1){
		printf("who can kill me?\n");
		sleep(1);
	}
	return 0;
}









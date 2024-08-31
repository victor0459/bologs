#include<stdio.h>
#include<unistd.h>
#include<signal.h>

int main(){
	//信号集
	sigset_t pend,sigproc;
	sigemptyset(&sigproc);//先清空
	sigaddset(&sigproc,SIGINT);//阻塞2号信号
	sigprocmask(SIG_BLOCK,&sigproc,NULL);
	while(1){
		sigpending(&pend);
		int i=1;
		for(i=1;i<32;++i){
			if(sigismember(&pend,i)==1){
				printf("1");
			}else{
				printf("0");
			}

		}
		printf("\n");
		sleep(1);
	}
	return 0;
}

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include"mysql.h"

#define _HOST_ "127.0.0.1"
#define _USER_ "root"
#define _PASSWD_ "root"
#define _DBNAME_ "scott"

int main(){
	//init
	MYSQL*mysql=mysql_init(NULL);
	if(mysql==NULL){
		printf("init err\n");
		exit(1);
	}
	//real_connnect
	mysql=mysql_real_connect(mysql,_HOST_,_USER_,_PASSWD_,_DBNAME_,0,NULL,0);
	if(mysql==NULL){
		printf("connect err\n");
		exit(1);
	}
	printf("hello mysql!\n");
	//close 关闭数据库
	mysql_close(mysql);
	return 0;

}

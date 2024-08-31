#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include"cJSON.h"
int main(int argc,const char*argv[]){
	//创建对象
	cJSON*obj=cJSON_CreateObject();	
	//创建子对象
	cJSON*subObj=cJSON_CreateObject();
	//obj添加key value
	cJSON_AddItemToObject(subObj,"factory",cJSON_CreateString("一汽大众"));
	cJSON_AddItemToObject(subObj,"last",cJSON_CreateNumber(31));
	cJSON_AddItemToObject(subObj,"price",cJSON_CreateNumber(83));
	cJSON_AddItemToObject(subObj,"sell",cJSON_CreateNumber(49));
	cJSON_AddItemToObject(subObj,"sum",cJSON_CreateNumber(80));
	//创建json数组
	cJSON*array=cJSON_CreateArray();
	cJSON_AddItemToArray(array,cJSON_CreateNumber(123));
	cJSON_AddItemToArray(array,cJSON_CreateNumber(1));
	cJSON_AddItemToArray(array,cJSON_CreateString("hello,world"));
	//数组中的对象
	cJSON*subsub=cJSON_CreateObject();
	cJSON_AddItemToObject(subsub,"梅赛德斯奔驰",cJSON_CreateString("心所向,持以恒,愿三岔星辉照亮你的事业和前程"));
	cJSON_AddItemToArray(array,subsub);
	cJSON_AddItemToObject(subObj,"other",array);
	cJSON_AddItemToObject(obj,"奔驰",subObj);
	//数据格式化
	char*data=cJSON_Print(obj);
	FILE*fp=fopen("1.json","w");
	fwrite(data,sizeof(char),strlen(data)+1,fp);
	fclose(fp);
	return 0;
}

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<mxml.h>

int main(int argc,char*argv[])
{
	//头文件 创建
	mxml_node_t*root=mxmlNewXML("1.0");
	//根标签
	mxml_node_t* china=mxmlNewElement(root,"china");
	//子标签--city
	mxml_node_t* city=mxmlNewElement(china,"city");
	mxml_node_t* info=mxmlNewElement(city,"name");
	//标签赋值
	mxmlNewText(info,0,"北京");
	//设置属性
	mxmlElementSetAttr(info,"isbig","Yes");
	//面积
	info=mxmlNewElement(city,"area");
	mxmlNewText(info,0,"16410 平方公里");
	//人口
	info=mxmlNewElement(city,"population");
	mxmlNewText(info,0,"2171万人");
	//gdp
	info=mxmlNewElement(city,"GDP");
	mxmlNewText(info,0,"24541亿元");
	
	//子标签--city
	city=mxmlNewElement(china,"city");
	info=mxmlNewElement(city,"name");
	//标签赋值
	mxmlNewText(info,0,"东京");
	//设置属性
	mxmlElementSetAttr(info,"isbig","No");
	//面积
	info=mxmlNewElement(city,"area");
	mxmlNewText(info,0,"2188 平方公里");
	//人口
	info=mxmlNewElement(city,"population");
	mxmlNewText(info,0,"3670万人");
	//gdp
	info=mxmlNewElement(city,"GDP");
	mxmlNewText(info,0,"31700亿元");
	//数据保存到磁盘文件
	FILE*fp=fopen("china.xml","w");
	mxmlSaveFile(root,fp,MXML_NO_CALLBACK);
	fclose(fp);
	mxmlDelete(root);
	return 0;
}


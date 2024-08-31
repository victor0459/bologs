#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

void test01() {
	//int  &ref = 10; 引用不合法的内存 不可以
	const int& ref = 10;//加入const后的处理方式 int tmp=10;const int &ref=tmp;
	int* p =(int* )&ref;
	*p = 1000;
	*p=200;
	*p=2000;
	cout << "ref=" << ref << endl;
}
//常量引用场景 用来修饰形参
 


int main() {
	test01();
	return 0;
}

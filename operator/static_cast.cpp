#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
//静态类型转换
//基础数据类型
void test1() {
	char a = 'a';
	double d = static_cast<double>(a);
	cout << "d=" << d << endl;
}
//父子之间转换
class Base{};
class Child:public Base{};
class Other{};
void test2() {
	Base* base = NULL;
	Child* child = NULL;
	//base->child 向下 不安全
	Child* child2 = static_cast<Child*>(base);
	//child->base 向上 安全
	Base* base2 = static_cast<Base*>(child);
	//无关系转换 无效
	Other* other = static_cast<Other*>(base);
}
int main() {
	test1();
	return 0;
}

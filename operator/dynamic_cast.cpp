#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
void test01() {
	//基础数据类型
	char c = 'a';
	//不可以转换  失去精度
	//double b = dynamic_cast<double>(c);
}
class Base{
	virtual void func() {};
};
class Child:public Base{
	virtual void func() {};
};
class Other{};

void test() {
	Base* base = NULL;
	Child* child = NULL;
	//child->base* 安全
	Base* base2 = dynamic_cast<Base*>(child);


	//base->child2*不安全
	//Child* child2 = dynamic_cast<Child*>(base);
	//dynamic_cast  如果发生了多态，那么可以让基类转成派生类 向下转换
	//发生了多态
	Base* base3 = new Child;
	Child* child3 = dynamic_cast<Child*>(base);
}


int main() {

	return 0;
}

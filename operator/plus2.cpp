#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
class Person {
public:
	Person() {};
	Person(int a, int b):m_A(a),m_B(b){}

	//+号运算符重载 成员函数
	/*Person operator+(Person& p) {
		Person tmp;
		tmp.m_A = this->m_A + p.m_A;
		tmp.m_B = this->m_B + p.m_B;
		return tmp;
	}*/
	int m_A;
	int m_B;
};
class MyInteger {
public:
	MyInteger() {
		m_Num = 0;
	}
	//前置++
	MyInteger& operator++() {
		this->m_Num++;
		return *this;
	}
	//后置++
	MyInteger operator++(int) {
		//先保存一下目前的数据
		MyInteger tmp = *this;
		m_Num++;
		return tmp;
	}
	int m_Num;
};
Person operator+(Person& p1, Person& p2) {
	Person tmp;
	tmp.m_A = p1.m_A + p2.m_A;
	tmp.m_B = p1.m_B + p2.m_B;
	return tmp;
}
ostream& operator<<(ostream& cout, Person& p1) {
	cout << "m_A=" << p1.m_A << " m_B=" << p1.m_B;
	return cout;
}
ostream& operator<<(ostream& cout, MyInteger& myInt) {
	cout << myInt.m_Num;
	return cout;
}
void test() {
	Person p1(10, 10);
	Person p2(10, 20);
	Person p3 = p1 + p2;
	//cout << "p3的m_A：" << p3.m_A << endl;
	//cout << "p3的m_B：" << p3.m_B << endl;
	cout << p3.m_B<<endl;
}
void test01() {
	MyInteger myInt;
	//++myInt;
	//cout << ++myInt << endl;
	myInt++;
	cout << myInt << endl;
}
int main() {
	test01();
}

#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
class Person {
public:
	Person(int age) {
		this->m_Age = age;
	}
	void showAge() {
		cout << "年龄为" << this->m_Age << endl;
	}
	~Person() {
		cout << "person 析构调用" << endl;
	}
	int m_Age;
};

//只能指针 主要用来托管自定义类型对象 让对象进行自动释放
class smartPointer {
public:
	smartPointer(Person* person) {
		this->person = person;
	}
	Person* operator->() {
		return this->person;
	}
	Person& operator*() {
		return *this->person;
	}
	~smartPointer() {
		cout << "智能指针析构" << endl;
		if (this->person != NULL) {
			delete this->person;
			this->person = NULL;
		}
	}
private:
	Person* person;
};
void test() {
	//Person* p1 = new Person(10);
	//delete p1;
	smartPointer sp(new Person(10));
	//sp->showAge();
	//(*sp).showAge();
}
int main() {


	test();
	return 0;
}

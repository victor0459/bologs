#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Animal {
public:
	virtual void speak() {
		cout << "动物在说话" << endl;
	}
	/*virtual~Animal() {
		cout << "animal析构调用" << endl;
	}*/
	//1纯虚析构需要类内声明 类外实现
	//2如果函数中出现纯虚析构函数 那么这个类也算抽象类
	//3抽象类 不可实例化对象
	virtual ~Animal() = 0;
};
Animal::~Animal() {
	cout << "animal纯虚析构调用" << endl;
}
class Cat :public Animal {
public:
	Cat(const char* name) {
		this->m_Name = new char[strlen(name) + 1];
		strcpy(this->m_Name, name);
	}
	virtual void speak() {
		cout << "小猫在说话" << endl;
	}
	~Cat() {
		cout << "cat析构调用" << endl;
		if (this->m_Name != NULL) {
			delete[] this->m_Name;
			this->m_Name = NULL;
		}
	}
	char* m_Name;
};
void test() {
	Animal* animal = new Cat("tom");
	animal->speak();
	//注意 普通析构是不会调用子类的析构函数
	//利用虚析构来解决这个问题
	delete animal;
}
int main() {

	test();
	return 0;
}

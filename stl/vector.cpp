#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<vector>
#include<algorithm>
#include<string>
//vector容器

class Person {
public:
	Person(string name, int age) {
		this->m_Name = name;
		this->m_Age = age;
	}

	string m_Name;
	int m_Age;
};

void myPrint(int v) {
	cout << v << endl;
}

//迭代器 遍历功能 用指针理解
//普通指针也是一种迭代器
void test01() {
	vector<int> v;//声明一个容器 这个容器存放int类型的数据 对象名v
	//向容器中加入数据
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	//遍历容器中的数据 利用迭代器
	/*vector<int>::iterator itBegin = v.begin();
	vector<int>::iterator itEnd = v.end();
	while (itBegin != itEnd) {
		cout << *itBegin << endl;
		itBegin++;
	}*/
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
		cout << *it << endl;
	}
	//第三种方式 利用算法
	for_each(v.begin(), v.end(), myPrint);
}
void test02() {
	Person p1("james", 20);
	Person p2("allson", 18);
	Person p3("kuli", 27);
	Person p4("dulante",29);
	vector<Person> v;
	v.push_back(p1.m_Name);
	v.push_back(p2);
	v.push_back(p3);
	v.push_back(p4);

	//遍历
	for (vector<Person>::iterator it = v.begin(); it != v.end(); it++) {
		cout << "姓名" << (*it).m_Name <<"年龄"<<it->m_Age <<endl;
	}
}
void test03() {
	
	Person* p1 = new Person("james", 20);
	Person* p2 = new Person("allson", 18);
	Person* p3 = new Person("kuli", 27);
	Person* p4 = new Person("dulante", 29);
	vector<Person*>v;
	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);
	v.push_back(p4);
	for (vector<Person*>::iterator it = v.begin(); it != v.end(); it++) {
		cout << "姓名" << (*it)->m_Name<< "年龄" << (*(*it)).m_Age << endl;
	}
	for (vector<Person*>::iterator it = v.begin(); it != v.end(); it++) {
		delete *it;
	}
}
//容器嵌套容器
void test05() {
	vector<vector<int>>v;
	vector<int>v1;
	vector<int>v2;
	vector<int>v3;
	for (int i = 0; i < 5; i++) {
		v1.push_back(i);
		v2.push_back(i + 10);
		v3.push_back(i + 100);
	}
	v.push_back(v1);
	v.push_back(v2);
	v.push_back(v3);
	//遍历所有的数据
	for (vector<vector<int>>::iterator it = v.begin(); it != v.end(); it++) {
		for (vector<int>::iterator vit = (*it).begin(); vit != (*it).end(); vit++) {
			cout << *vit << " ";
		}
		cout << endl;
	}
}
int main() {
	//test01();
	//test02();
	//test03();
	test05();
	return 0;
}

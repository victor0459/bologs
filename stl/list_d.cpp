#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<queue>
#include<list>
#include<algorithm>
#include<string>
using namespace std;
class Person {
public:
	Person(string name, int age) {
		this->m_Name = name;
		this->m_Age = age;
	}
	bool operator==(Person& p) {
		if (this->m_Name == p.m_Name && this->m_Age == p.m_Age) {
			return true;
		}
		return false;
	}
	string m_Name;
	int m_Age;
};
void test01() {
	queue<int>q;
	q.push(10);
	q.push(20);
	q.push(30);
	while (!q.empty()) {
		cout << "对头:" << q.front() << endl;
		cout << "对尾:" << q.back() << endl;
		q.pop();//弹出队头
	}
}
void printList(list<int>& L) {
	for (list<int>::iterator it = L.begin(); it != L.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
}
void RPrintList(list<int>& L) {
	for (list<int>::reverse_iterator it = L.rbegin(); it != L.rend(); it++) {
		cout << *it << " ";
	}
	cout << endl;
}
void test02() {
	//list是双向循环链表
	list<int> l3;
	for (int i = 0; i < 10; i++) {
		l3.push_back(i);
	}
	//list<int>::_Nodeptr node = myList._Myhead->_Next;
	printList(l3);
	RPrintList(l3);
	//不支持随机访问

	//插入
	l3.push_back(99);//尾
	l3.push_front(66);
	printList(l3);
	//删除 头尾
	l3.pop_back();
	l3.pop_front();
	printList(l3);
	l3.insert(l3.begin(), 1000);
	l3.remove(5);//删除为5的元素
	printList(l3);

	cout << "l3的大小" << l3.size() << endl;
}
bool myCompare(int v1, int v2) {
	return v1 > v2;
}
void test03() {
	list<int>l;
	l.push_back(10);
	l.push_back(30);
	l.push_back(20);
	l.push_back(40);
	l.reverse();
	printList(l);
	//不支持随机访问的迭代器 不支持系统提供的算法
	//系统不支持  这个类会提供
	l.sort();
	printList(l);
	l.sort(myCompare);
	printList(l);
}
bool myComparePerson(Person& p1, Person& p2) {
	if (p1.m_Age > p2.m_Age) {
		return true;
	}
	return false;
}
void test04() {
	//自定义数据类型
	Person p1("刘备", 20);
	Person p2("阿狸", 28);
	Person p3("孙尚香", 18);
	Person p4("赵云", 25);
	list<Person>l;
	l.push_back(p1);
	l.push_back(p2);
	l.push_back(p3);
	l.push_back(p4);
	l.sort(myComparePerson);
	for (list<Person>::iterator it = l.begin(); it != l.end(); it++) {
		cout << "姓名:" << (*it).m_Name << "年龄:" << (*it).m_Age << endl;
	}
	l.remove(p2);
	for (list<Person>::iterator it = l.begin(); it != l.end(); it++) {
		cout << "姓名:" << (*it).m_Name << "年龄:" << (*it).m_Age << endl;
	}
}
int main() {
	//test01();
	//test02();
	//test03();
	test04();
	return 0;
}

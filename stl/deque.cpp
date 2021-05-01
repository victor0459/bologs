#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<deque>
using namespace std;
void printDeque(const deque<int>& a) {
	//迭代器reverse_iterator const_iterator只读迭代器
	
	for (deque<int>::const_iterator it = a.begin(); it != a.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
}
void test01() {
	deque<int>d;
	d.push_back(10);
	d.push_back(20);
	d.push_back(30);
	d.push_back(40);
	printDeque(d);
	deque<int>d2(d.begin(), d.end());
	d2.push_back(10000);
	printDeque(d2);
	d.swap(d2);

}
void test02() {
	deque<int>d;
	d.push_back(10);
	d.push_back(20);
	d.push_back(30);
	d.push_front(40);
	printDeque(d);
	//删除 头删
	d.pop_back();//尾删
	d.pop_front();//头删
	cout << "front:" << d.front() << endl;
	cout << "back:" << d.back() << endl;
	//插入
	deque<int>d2;
	d2.push_back(1);
	d2.push_back(2);
	d2.push_back(3);
	d2.insert(d2.begin(), d.begin(), d.end());
	printDeque(d2);

	//
}
int main() {
	//test01();
	test02();
	return 0;
}





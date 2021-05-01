#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include<vector>
#include<algorithm>
#include<string>

void printVector(vector<int>& v) {
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
	cout << "容器容量"<<v.capacity() << endl;
}

void test01() {
	//常用api
	vector<int>v;
	//构造函数
	int arr[] = { 1,2,3,4,5,6,7,8 };
	vector<int> v1(arr, arr + sizeof(arr) / sizeof(int));
	vector<int>v2(v1.begin(), v1.end());
	printVector(v2);
	//构造函数 将n个m拷贝
	vector<int>v3(10, 100);
	printVector(v3);
	v3.resize(10, 0);
	printVector(v3);
	//容器大小
	v3.size();
}
void test02() {
	//使用swap收缩空间
	
	vector<int>v;
	for (int i = 0; i < 10; i++) {
		v.push_back(i);
	}
	v.resize(3);
	//收缩空间
	vector<int>(v).swap(v);
}
void test03() {
	vector<int>v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	cout << "v的front:" << v.front() << endl;
	cout << "v的back:" << v.back() << endl;
	v.insert(v.begin(), 2,100);//参数一 迭代器 参数2 N 参数三 具体插入的内容
	printVector(v);
	v.pop_back();//尾删
	printVector(v);
	v.erase(v.begin(), v.end());//删除某个区间的数
	if (v.empty()) {
		cout << "为空" << endl;
	}
	v.clear();//清空所有数据
}
void test04() {
	vector<int>v;
	for (int i = 0; i < 10; i++) {
		v.push_back(i);
	}
	//逆序遍历
	for (vector<int>::reverse_iterator it = v.rbegin(); it != v.rend(); it++) {
		cout << *it << " ";
	}
	cout << endl;
	//vector迭代器是随机访问的迭代器 支持跳跃式访问
	vector<int>::iterator itBegin = v.begin();
	itBegin = itBegin + 3;//此步不报错 可以支持随机访问
}
int main() {

	//test01();
	//test03();
	test04();
	return 0;
}

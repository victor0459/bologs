#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<set>
using namespace std;
void printSet(set<int>& s) {
	for (set<int>::iterator it = s.begin(); it != s.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
}
void test01() {
	//关联式容器 会自动排好序 从小到大排序
	set<int>s1;
	s1.insert(5);
	s1.insert(1);
	s1.insert(9);
	s1.insert(3);
	s1.insert(7);
	printSet(s1);
	//删除
	s1.erase(s1.begin());
	//s1.erase(3);
	//printSet(s1);
	set<int>::iterator pos = s1.find(3);
	cout << "1的个数:" << s1.count(1) << endl;

	set<int>::iterator it = s1.lower_bound(3);//返回元素3的迭代器
	cout << *it << endl;
	set<int>::iterator it2 = s1.upper_bound(3);//返回元素3后面一个元素的迭代器
	cout << *it2 << endl;
	//返回上下限两个迭代器
	pair<set<int>::iterator, set<int>::iterator> ret = s1.equal_range(3);
	//获取第一个值
	cout << "第一个值为:" << *(ret.first) << endl;
	cout << "第二个值为:" << *(ret.second) << endl;
}
void test02() {
	//对组
	//创建对组  第一种方式
	pair<string, int> p(string("tom"), 100);
	//取值 
	cout << "姓名：" << p.first << endl;
	cout << "年龄：" << p.second << endl;
	//第二种方式
	pair<string, int> p2 = make_pair("jerry", 58);
	cout << "姓名：" << p2.first << endl;
	cout << "年龄：" << p2.second << endl;
}
class Compare {
public:
	bool operator()(const int v1, const int v2)const  {
		return v1 > v2;
	}
};
void test03() {
	//指定set的排序规则
	set<int,Compare>s1;
	s1.insert(5);
	s1.insert(1);
	s1.insert(9);
	s1.insert(3);
	s1.insert(7);
	for (set<int, Compare>::iterator it = s1.begin(); it != s1.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
}
int main() {
	//test01();
	//test02();
	test03();
	return 0;
}

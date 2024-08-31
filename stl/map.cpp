#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<map>
using namespace std;

void test01() {
	map<int, int>m;
	//插入值 4种
	//第一个
	m.insert(pair<int, int>(1, 10));
	//第二种
	m.insert(make_pair(2, 20));
	//第三种
	m.insert(map<int, int>::value_type(3, 30));
	//第四种
	m[4] = 40;
	for (map<int, int>::iterator it = m.begin(); it != m.end(); it++) {
		cout << "key:" << it->first << "value:" << it->second << endl;
	}
	//删除
	m.erase(1);
	//迭代器
	map<int, int>::iterator pos = m.find(1);
	cout << "key:" << pos->first << "value:" << pos->second << endl;
	cout << m.count(1) << endl;
	map<int, int>::iterator ret = m.lower_bound(3);
	map<int, int>::iterator ret = m.upper_bound(3);
	pair<map<int, int>::iterator, map<int, int>::iterator>ret2 = m.equal_range(3);
}

int main() {
	test01();
	return 0;
}

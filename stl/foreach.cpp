#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;
void printV(int v) {
	cout << v << endl;
}
struct myprint {
	void operator()(int v) {
		cout << v << endl;
		m_Count++;
	}
	int m_Count;
};

void test01() {
	vector<int>v;
	for (int i = 0; i < 10; i++) {
		v.push_back(i);
	}
	//for_each(v.begin(), v.end(), printV);
	myprint print2=for_each(v.begin(), v.end(), myprint());
	//foreach 有返回值 可以保存内部记录
	cout << "调用次数:" << print2.m_Count << endl;
}
class Transfrom {
public:
	int operator()(int val) {
		return val;
	}
};
void test02() {
	//transform 第一种用法 搬运功能
	vector<int>v;//作为源容器
	for (int i = 0; i < 10; i++) {
		v.push_back(i);
	}
	vector<int>vTarget;
	//注 需要开辟内存
	vTarget.resize(v.size());
	transform(v.begin(), v.end(), vTarget.begin(), Transfrom());
	for_each(vTarget.begin(), vTarget.end(), [](int val) {cout << val << " "; });
}
int main() {
	test01();
	//test02();
	return 0;
}


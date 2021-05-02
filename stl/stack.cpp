#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stack>
using namespace std;

void test01() {
	stack<int>s;
	s.push(10);
	s.push(20);
	s.push(30);
	s.push(40);
	while (s.size() != 0) {
		cout << "栈顶为" << s.top() << endl;
		//弹出栈顶元素
		s.pop();
	}

}

int main() {
	test01();
	return 0;
}

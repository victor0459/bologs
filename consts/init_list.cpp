#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

class Phone {
public:
	Phone() {
		cout << "手机的默认构造函数调用" << endl;

	}
	Phone(string name) :m_PhoneName(name){
	}
	~Phone() {
		cout << "手机的析构函数调用" << endl;
	}
	string m_PhoneName;
};
class Game {
public:
	Game() {
		cout << "Game的默认构造函数调用" << endl;

	}
	Game(string name) :m_GameName(name) {

	}
	~Game() {
		cout << "Game的析构函数调用" << endl;
	}
	string m_GameName;
};

class Person {
public:
	Person() {
		cout << "Person的默认构造函数调用" << endl;

	}
	Person(string name,string phoneName,string gameName):m_Name(name),m_Phone(phoneName),m_Game(gameName)  {

	}
	~Person() {
		cout << "Person的析构函数调用" << endl;
	}

	string m_Name;
	Phone m_Phone;
	Game m_Game;
};

void test() {
	Person p;
	//p.m_Phone.m_PhoneName = "三星";
	//p.m_Game.m_GameName = "吃鸡";
	p.m_Game.m_GameName="LOL";
}

int main() {

	test();
	return 0;
}

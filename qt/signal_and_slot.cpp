#include "widget.h"
#include<QPushButton>
#include<QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    zt=new Teacher(this);
    st=new Student(this);
    //连接老师和学生
    //connect(zt,&Teacher::hungry,st,&Student::treat);
    //函数指针 指向函数地址
    //void(Teacher::*teacherSignal)(QString)=&Teacher::hungry;
    //void(Student::*stSolt)(QString)=&Student::treat;

    //connect(zt,teacherSignal,st,stSolt);
    //下课
    //classIsOver();
    //点击按钮下课

    //QPushButton*btn=new QPushButton("下课",this);
    //void(Teacher::*noteacherSignal)(void)=&Teacher::hungry;
    //void(Student::*nostSolt)(void)=&Student::treat;
    //信号连接信号
    //connect(btn,&QPushButton::clicked,zt,noteacherSignal);
    //connect(zt,noteacherSignal,st,nostSolt);
    resize(600,400);

    //Qt4版本 信号和槽
    //connect(zt,SIGNAL(hungry(QString)),st,SLOT(treat(QString)));


    QPushButton*btn2=new QPushButton("aaa",this);
    [=](){
        btn2->setText("bbb");
    }();
    //mutable关键字 用于值传递的变量 进行修改
    QPushButton*btn3=new QPushButton("ccc",this);
    QPushButton*btn4=new QPushButton("bbb",this);
    btn3->move(100,100);
    btn4->move(200,200);
    int m=10;
    connect(btn3,&QPushButton::clicked,this,[m]()mutable{m=100+10;qDebug()<<m;});

    connect(btn3,QPushButton::clicked,this,[=](){
        btn3->setText("hhh");
    });
}
void Widget::classIsOver(){
    //触发老师饿了的信号
    //老师饿了的信号属于自定义信号 触发自定义信号关键字emit
    emit zt->hungry();
    emit zt->hungry("京酱肉丝");
}

Widget::~Widget()
{

}


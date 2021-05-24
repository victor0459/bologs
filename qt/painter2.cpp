#include "widget.h"
#include "ui_widget.h"
#include<QPainter>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}
void Widget::paintEvent(QPaintEvent *){
    //创建画家
    QPainter painter(this);
    //设置画笔颜色
    QPen pen(QColor(255,0,0));
    pen.setWidth(3);
    //使用这支笔
    painter.setPen(pen);
    //画刷填充颜色
    QBrush brush(Qt::darkBlue);
    painter.setBrush(brush);
    //画线
    painter.drawLine(QPoint(0,0),QPoint(100,100));

    //画椭圆
    painter.drawEllipse(QPoint(100,100),50,50);
    //高级设置  抗锯齿 效率低
    painter.setRenderHint(QPainter::Antialiasing);
    //画矩形
    painter.drawRect(QRect(10,10,50,50));
    //画字体
    painter.drawText(QRect(10,100,50,50),"淘宝");
}

Widget::~Widget()
{
    delete ui;
}


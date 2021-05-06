#include "widget.h"
#include "ui_widget.h"
#include<QFileDialog>
#include<QMessageBox>
#include<QFile>
#include<QTextCodec>
#include<QTextStream>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //点击按钮 选取文件
    connect(ui->pushButton,&QPushButton::clicked,[=](){
        QString path=QFileDialog::getOpenFileName(this,"打开文件","C:\\Users\\lenovo\\Desktop");
        if(path.isEmpty()){
            QMessageBox::warning(this,"警告","打开失败");
        }else{
            //将路径 放入到lineEdit
            ui->lineEdit->setText(path);

            //QTextCodec *codec=QTextCodec::codecForName("gbk");
            //读取文件
            QFile file(path);//参数路径名称
            //指定打开方式
            file.open(QIODevice::ReadOnly);
            QByteArray array;
            array=file.readAll();
            //设置到文本编辑框中
            ui->textEdit->setText(array);
            //关闭文件
            file.close();

            //写文件
//            file.open(QFileDevice::Append);
//            file.write("hello world");
//            file.close();

            //文件信息
            QFileInfo info(path);
            info.fileName();//名字
            info.size();//大小
        }

    });
    //文件流读写文件
    //分类 文本流（基础数据类型） 和数据流（图片）
    //文本流
    QFile file("a.txt");
    file.open(QFileDevice::WriteOnly);
    QTextStream stream(&file);
    //写数据
    stream<<QString("hello world")<<123456;
    file.close();

    //读数据
    file.open(QFileDevice::ReadOnly);
    QString str;
    //stream>>str;//读取到空格就结束
    stream.readAll();
    qDebug()<<str;

    //数据流
    QFile file("a.txt");
    file.open(QFileDevice::WriteOnly);
    QDataStream stream(&file);
    stream<<QString("hello world")<<123456;
    file.close();
    //读数据
    file.open(QFileDevice::ReadOnly);
    QString str;
    stream>>str;
    qDebug()<<endl;
}

Widget::~Widget()
{
    delete ui;
}


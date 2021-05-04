#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QDialog>
#include<QMessageBox>
#include<QColorDialog>
#include<QFileDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //点击新建菜单项 弹出对话框
    connect(ui->actionNew,&QAction::triggered,this,[=](){
        qDebug()<<"弹出对话框";
        //对话框有两种
        //-----------模态对话框------------
        //QDialog dig(this);
        //dig.resize(200,100);
        //dig.exec();//阻塞

        //-----------非模态对话框------------
        //QDialog dig2(this);//栈上
        //QDialog*dig2=new QDialog(this);
        //dig2->resize(200,100);
        //dig2->show();
        //需要给dig2设置属性 关闭窗口释放掉
        //dig2->setAttribute(Qt::WA_DeleteOnClose);

        //-----------系统自带的对话框-------------
        //1 错误对话框
        //QMessageBox::critical(this,"错误!","critical");
        //2 信息对话框
        //QMessageBox::information(this,"信息","info");
        //3 询问对话框
        //返回值是如QMessageBox::Yes
        //QMessageBox::question(this,"问题","发财啦",QMessageBox::Yes|QMessageBox::Save|QMessageBox::Cancel,QMessageBox::Yes);
        //4警告
        //QMessageBox::warning(this,"警告","warning");

        //-----------选择颜色对话框---------------
        //QColor color=QColorDialog::getColor(QColor(255,0,0));
        //qDebug()<<color.red()<<color.green();
        //------------文件对话框--------------
        QString path=QFileDialog::getOpenFileName(this,"打开文件","C:\\Users\\lenovo\\Desktop","(*.txt *.png)");
        qDebug()<<path;

    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <qtextstream.h>
#include <QPlainTextEdit>
//#include <QtDebug>
//#include <iostream>
//using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initFileData();

    //信号与槽链接
    //---------------------------文件部分----------------------------------
    connect(ui->action_NewFile,SIGNAL(triggered(bool)),this,SLOT(newFile()));
    connect(ui->action_Open,SIGNAL(triggered(bool)),this,SLOT(openFile()));
    connect(ui->action_Save,SIGNAL(triggered(bool)),this,SLOT(saveFile()));
    connect(ui->action_SaveAs,SIGNAL(triggered(bool)),this,SLOT(saveAsFile()));

    //---------------------------编辑部分----------------------------------
    connect(ui->action_Undo,SIGNAL(triggered(bool)),this,SLOT(undo()));
    connect(ui->action_Redo,SIGNAL(triggered(bool)),this,SLOT(redo()));
    connect(ui->action_AllSelect,SIGNAL(triggered(bool)),this,SLOT(allSelect()));
    connect(ui->action_Cut,SIGNAL(triggered(bool)),this,SLOT(cut()));
    connect(ui->action_Copy,SIGNAL(triggered(bool)),this,SLOT(copy()));
    connect(ui->action_Paste,SIGNAL(triggered(bool)),this,SLOT(paste()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//初始化文件信息
void MainWindow::initFileData()
{
    fileName=tr("Untitled");
    filePath=tr("~/Desktop/Untitled.cpp");
    isSaved=true;
    isRunning=false;
}

//--------新建文件----------
//1.新建主窗口对象
//2.确定新窗口位置
//3.新建窗口
//---------LCH------------
void MainWindow::newFile()
{ 
    MainWindow *newWindow = new MainWindow;
    QRect newPosition = this->geometry();
    newWindow->setGeometry(newPosition.x()+10,newPosition.y()+10,newPosition.width(),newPosition.height());
    newWindow->show();
    Flag_isNew = true;
    Flag_isOpen = true;
}

//-------------打开文件-------------------
//打开文件逻辑：
//首先获取文件名，判断文件名是否为空（是否直接关闭对话框）
//在判断文件是否可读
//如果没到文件末尾则一直读出到palintextedit中
//Flag_isOpen = true;
//Flag_isNew = false;
//----------------LCH-------------------
void MainWindow::openFile()
{
    //获取文件名,默认只能打开.cpp,.h,.c
    QString filename = QFileDialog::getOpenFileName(this,tr("打开文件"),tr(""),tr("C++ source Files(*.cpp *.c *.h)"));
    if(filename.isEmpty()){  //如果用户直接关闭对话框则文件名为空
        return ;
    }
    else{
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
            QMessageBox::warning(this,tr("错误"),tr("打开文件失败"));
            return ;
        }
        else {
            if(!file.isReadable()){
                QMessageBox::warning(this,tr("错误"),tr("文件不可读"));
            }
            else{
                QTextStream textStream(&file);//读取文件
                while(!textStream.atEnd()){
                    ui->plainTextEdit->setPlainText(textStream.readAll());
                }
                ui->plainTextEdit->show();
                file.close();
                Flag_isOpen = true;
                Flag_isNew = false;
                Last_FileName = filename;
            }
        }
    }
}

//----------保存文件------------
//保存文件逻辑：
//首先确定：
//新建文件后Flag_isNew=true  Flag_isOpen=true
//打开文件后Flag_isNew=false  Flag_isOpen=true
//如果是新文件：
//1.判断文件是否为空
//2.获取保存文件名，防止不命名直接关闭
//3.获取保存时的文件内容
//-----------LCH---------------

void MainWindow::saveFile()
{
    if(Flag_isNew){                  //如果新文件标记位为1，则弹出保存文件对话框
        if(ui->plainTextEdit->toPlainText() == ""){
            QMessageBox::warning(this,tr("警告"),tr("内容不能为空!"),QMessageBox::Ok);
        }
        else{
            QFileDialog fileDialog;
            QString str = fileDialog.getSaveFileName(this,tr("Open File"),"/home",tr("Text File(*.cpp *.c *.h)"));
            if(str == ""){
                return;
            }
            QFile filename(str);
            if(!filename.open(QIODevice::WriteOnly | QIODevice::Text)){
                QMessageBox::warning(this,tr("错误"),tr("打开文件失败"),QMessageBox::Ok);
                return;
            }
            else{
                QTextStream textStream(&filename);
                QString str = ui->plainTextEdit->toPlainText();
                textStream<<str;
                Last_FileContent = str;
            }
            QMessageBox::information(this,"保存文件","保存文件成功",QMessageBox::Ok);
            filename.close();
            Flag_isNew = false;
            Flag_isOpen = true;//新文件标记位记为0
            Last_FileName = str;//保存文件内容
        }
    }
    else{                        //否则，新文件标记位是0，代表是旧文件，默认直接保存覆盖源文件
        if(Flag_isOpen){         //判断是否创建或打开了一个文件
            QFile file(Last_FileName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
                QMessageBox::warning(this,tr("警告"),tr("打开文件失败"));
                return;
            }
            else{
                QTextStream textStream(&file);
                QString str = ui->plainTextEdit->toPlainText();
                textStream<<str;
                Last_FileContent = str;
                file.close();
            }
        }
        else{
            QMessageBox::warning(this,tr("警告"),tr("请先创建或者打开文件"));
            return;
        }
    }

}

//-------另存为------------
//1.获取文件名
//2.判断文件名是否为空
//3.保存过程同上
//--------LCH------------
void MainWindow::saveAsFile()
{
    QFileDialog fileDialog;
    QString filename = fileDialog.getSaveFileName(this,tr("选择保存路径与文件名"),"/home","C++ source Files(*.cpp *.c *.h)");
    if(filename.isEmpty()){
        return ;
    }
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        QMessageBox::warning(this,tr("错误"),tr("保存文件失败"));
        return;
    }
    else {
        QTextStream textStream(&file);
        QString str = ui->plainTextEdit->toPlainText();
        textStream<<str;
        QMessageBox::warning(this,tr("提示"),tr("保存文件成功"));
        Last_FileContent=str;
        Last_FileName = filename;
        Flag_isNew = false;
        file.close();
    }
}


//BUG:保存提醒:有问题，不进入
void MainWindow::saveWarn(QCloseEvent *event)
{
    //qDebug()<<ui->plainTextEdit->toPlainText();
    if(ui->plainTextEdit->toPlainText() == Last_FileContent) //如果文本框的内容就是上次保存的文件内容，则接收信号，关闭
        event->accept();
    else{                                           //否则弹出警告，有修改的内容未保存
        if(QMessageBox::warning(this,tr("警告"),tr("文件还未保存,确定退出？"),QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes){
            event->accept();
        }
        else
            event->ignore();
    }
}


//撤销
void MainWindow::undo()
{
    ui->plainTextEdit->undo();
}
//重做
void MainWindow::redo()
{
    ui->plainTextEdit->redo();
}
//全选
void MainWindow::allSelect()
{
    ui->plainTextEdit->selectAll();
}
//剪切
void MainWindow::cut()
{
 ui->plainTextEdit->cut();
}
//复制
void MainWindow::copy()
{
 ui->plainTextEdit->copy();
}
//粘贴
void MainWindow::paste()
{
    ui->plainTextEdit->paste();
}




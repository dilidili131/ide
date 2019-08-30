#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <qtextstream.h>
#include <QPlainTextEdit>

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
    fileName=tr("Untitled.cpp");
    filePath=tr("~/Desktop/Untitled.cpp");
    isSaved=true;
    isRunning=false;
}

//新建文件
void MainWindow::newFile()
{
    //1.新建主窗口对象
    //2.确定新窗口位置
    //3.新建窗口
    MainWindow *newWindow = new MainWindow;
    QRect newPosition = this->geometry();
    newWindow->setGeometry(newPosition.x()-10,newPosition.y()-10,newPosition.width(),newPosition.height());
    newWindow->show();
    Flag_IsNew = true;
    Flag_IsNew = true;
}

//打开文件
void MainWindow::openFile()
{
    fileName = QFileDialog::getOpenFileName(this,tr("Open File"),tr(""),tr("Text File(*.cpp)"));
    if(fileName.isEmpty()){  //如果用户直接关闭对话框则文件名为空
        return ;
    }
    else{
        QFile file(fileName);
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
                Last_FileName = fileName;
            }
        }
    }
}

//保存文件
void MainWindow::saveFile()
{
    if(Flag_IsNew){//如果是新文件则弹出保存框
        if(ui->plainTextEdit->toPlainText().isEmpty()){
            QMessageBox::warning(this,tr("警告"),tr("内容不能为空!"),QMessageBox::Ok);
        }
        else{
            QFileDialog fileDialog;
            QString f_name = fileDialog.getSaveFileName(this,tr("Open File"),"/home",tr("Cpp File(*.cpp)"));
            if(f_name.isEmpty())
                return ;
            QFile filename(f_name);
            if(!filename.open(QIODevice::WriteOnly)|QIODevice::Text){
                QMessageBox::warning(this,tr("错误"),tr("保存文件失败"),QMessageBox::Ok);
                return ;
            }
            else{
                QTextStream textStream(&filename);
                QString str = ui->plainTextEdit->toPlainText();
                textStream<<str;
                Last_FileContent = str;
            }
            QMessageBox::information(this,"保存文件","保存文件成功",QMessageBox::Ok);
            filename.close();
            Flag_IsNew = false;
            Last_FileName = f_name;
        }
    }
    else{
        if(Flag_isOpen){
            QFile file(Last_FileName);
            if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
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

//撤销
void MainWindow::undo()
{

}
//重做
void MainWindow::redo()
{

}
//全选
void MainWindow::allSelect()
{

}
//剪切
void MainWindow::cut()
{

}
//复制
void MainWindow::copy()
{

}
//粘贴
void MainWindow::paste()
{

}

void MainWindow::on_textEdit_destroyed()
{

}

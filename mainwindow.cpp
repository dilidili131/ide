#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>

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
}

//打开文件
void MainWindow::openFile()
{

}

//保存文件
void MainWindow::saveFile()
{

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

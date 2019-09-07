#include "codeeditor.h"
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
    setCentralWidget(codeeditor);   //设主体为代码编辑器

    isChanged = false;

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

    //---------------------------编译部分-----------------------------------
    connect(ui->action_Run,SIGNAL(triggered(bool)),this,SLOT(comp()));
    connect(ui->action_Run_2,SIGNAL(triggered(bool)),this,SLOT(run()));
    connect(ui->plainTextEdit,SIGNAL(textChanged()),this,SLOT(on_changed()));//当文本内容发生变化时，触发on_changed函数

    //关闭分页
    connect(codeeditor->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(removeSubTab(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//初始化文件信息
void MainWindow::initFileData()
{
    fileName=tr("Untitled.cpp");
    isSaved=false;
    isRunning=false;
}
//void MainWindow::addNewtab()
//{
//    QWidget *widget = new QWidget();
//    QVBoxLayout *v = new QVBoxLayout();
//    CodeEditor c;
//    v->addWidget(c.geteditor());
//    v->addWidget(c.getconsole());
//    v->setContentsMargins(0,0,0,0);
//    v->setStretchFactor(c.geteditor(), 4);
//    v->setStretchFactor(c.getconsole(), 1);
//    widget->setLayout(v);

//    codeeditor->tabWidget->addTab(widget,"untitled.cpp");
//}
//--------新建文件----------
//1.新建主窗口对象
//2.确定新窗口位置
//3.新建窗口
//---------LCH------------
void MainWindow::newFile()
{
//    MainWindow *newWindow = new MainWindow;
//    QRect newPosition = this->geometry();
//    newWindow->setGeometry(newPosition.x()+10,newPosition.y()+10,newPosition.width(),newPosition.height());
//    newWindow->show();
    //addNewtab();
    QWidget *widget = new QWidget();
    QVBoxLayout *v = new QVBoxLayout();
    //CodeEditor c;
    v->addWidget(codeeditor->geteditor());
    v->addWidget(codeeditor->getconsole());
    v->setContentsMargins(0,0,0,0);
    v->setStretchFactor(codeeditor->geteditor(), 4);
    v->setStretchFactor(codeeditor->getconsole(), 1);
    widget->setLayout(v);

    codeeditor->tabWidget->addTab(widget,"untitled.cpp");
    Flag_isNew = true;
    Flag_isOpen = true;
    isChanged = false;
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
                    codeeditor->geteditor()->setText(textStream.readAll());
                    //codeeditor->geteditor()->setPlainText(textStream.readAll());
                }
                codeeditor->geteditor()->show();
                file.close();
                Flag_isOpen = true;
                Flag_isNew = false;
                Last_FileName = filename;

                QRegularExpression re(tr("(?<=\\/)\\w+\\.cpp|(?<=\\/)\\w+\\.c|(?<=\\/)\\w+\\.h"));
                fileName=re.match(filename).captured();
                //this->setWindowTitle(tr("IDE - ")+fileName);
                int index = codeeditor->tabWidget->currentIndex();
                codeeditor->tabWidget->setTabText(index,fileName);
                isSaved=true;
                isChanged = false;
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
        if(codeeditor->geteditor()->text() == ""){
            QMessageBox::warning(this,tr("警告"),tr("内容不能为空!"),QMessageBox::Ok);
        }
        else{
            QFileDialog fileDialog;
            QString str = fileDialog.getSaveFileName(this,tr("Open File"),fileName,tr("Text File(*.cpp *.c *.h)"));
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
                QString str = codeeditor->geteditor()->text();
                textStream<<str;
                Last_FileContent = str;
            }
            QMessageBox::information(this,"保存文件","保存文件成功",QMessageBox::Ok);
            filename.close();
            isSaved = true;
            QRegularExpression re(tr("(?<=\\/)\\w+\\.cpp|(?<=\\/)\\w+\\.c|(?<=\\/)\\w+\\.h"));
            fileName=re.match(str).captured();
            //this->setWindowTitle(tr("IDE - ")+fileName);
            int index = codeeditor->tabWidget->currentIndex();
            codeeditor->tabWidget->setTabText(index,fileName);
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
                QString str = codeeditor->geteditor()->text();
                textStream<<str;
                Last_FileContent = str;
                file.close();
                isSaved = true;
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
    QString filename = fileDialog.getSaveFileName(this,tr("选择保存路径与文件名"),fileName,"C++ source Files(*.cpp *.c *.h)");
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
        QString str = codeeditor->geteditor()->text();
        textStream<<str;
        QMessageBox::warning(this,tr("提示"),tr("保存文件成功"));
        Last_FileContent=str;
        Last_FileName = filename;
        Flag_isNew = false;
        file.close();
        isSaved = true;
        isChanged = false;
        QRegularExpression re(tr("(?<=\\/)\\w+\\.cpp|(?<=\\/)\\w+\\.c|(?<=\\/)\\w+\\.h"));
        fileName=re.match(filename).captured();
        //this->setWindowTitle(tr("IDE - ")+fileName);
        int index = codeeditor->tabWidget->currentIndex();
        codeeditor->tabWidget->setTabText(index,fileName);
    }
}


//BUG:保存提醒:有问题，不进入
void MainWindow::saveWarn(QCloseEvent *event){
  if(!isSaved){
      if(QMessageBox::Save==QMessageBox::question(this,tr("未保存就要退出？"),tr("当前文件没有保存，是否保存？不保存文件改动将会丢失"),QMessageBox::Save,QMessageBox::Cancel))
        saveFile();
      isSaved=true;
    }
}


//撤销
void MainWindow::undo()
{
    codeeditor->geteditor()->undo();
}
//重做
void MainWindow::redo()
{
    codeeditor->geteditor()->redo();
}
//全选
void MainWindow::allSelect()
{
    codeeditor->geteditor()->selectAll();
}
//剪切
void MainWindow::cut()
{
 codeeditor->geteditor()->cut();
}
//复制
void MainWindow::copy()
{
 codeeditor->geteditor()->copy();
}
//粘贴
void MainWindow::paste()
{
    codeeditor->geteditor()->paste();
}
void MainWindow::onChanged()
{
    isChanged = true;
}
//编译部分有个bug，把文件放到build-IDE文件夹里才能编译

void MainWindow::precomp()//预编译
{
    FILE *p = fopen(fileName.toStdString().data(),"r");
    if(p == NULL) return ;
    QString cmd = fileName +".c";
    FILE *p1 = fopen(cmd.toStdString().data(),"w");
    if(p1 == NULL) return ;
    QString str;
    while(!feof(p))
    {
        char buf[1024] = {0};
        fgets(buf,sizeof(buf),p);
        str += buf;
    }
    fputs(str.toStdString().data(),p1);
    fclose(p);
    fclose(p1);
}


//编译并运行按钮
void MainWindow::comp()
{
    saveFile();
    precomp();//自动以预编译
    QString cmd;
    const char *s = fileName.toStdString().data();
    cmd.sprintf("gcc -o %s.exe %s.c 2> err.txt",s,s);
    system(cmd.toStdString().data());//先编译

    cmd = fileName.replace("/","\\") + ".c";
    remove(cmd.toStdString().data());

    FILE *f = fopen("err.txt","r");
    if(f==NULL)
    {
        cmd = fileName + ".exe";
        system(cmd.toStdString().data());//再运行
    }
    else
    {
        QString str;
        while(!feof(f))
        {
            char buf[1024] = {0};
            fgets(buf,sizeof(buf),f);
            str += buf;
        }
        codeeditor->getconsole()->setText(str);
        //codeeditor->getconsole()->text(str);
    }
}

void MainWindow::run()
{
    QString cmd;
    cmd = fileName + ".exe";
    system(cmd.toStdString().data());

    remove(cmd.toStdString().data());
}

void MainWindow::removeSubTab(int index)//关闭分页
{
    saveFile();
    qDebug("kkk");
    codeeditor->tabWidget->removeTab(index);
}



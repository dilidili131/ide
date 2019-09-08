#include "codeeditor.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <qtextstream.h>
#include <QPlainTextEdit>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initFileData();
    setCentralWidget(codeeditor);   //设主体为代码编辑器

    //信号与槽链接
    //---------------------------文件部分----------------------------------
    connect(ui->action_NewFile,SIGNAL(triggered(bool)),this,SLOT(newFile()));
    connect(ui->action_Open,SIGNAL(triggered(bool)),this,SLOT(openFile()));
    connect(ui->action_Save,SIGNAL(triggered(bool)),this,SLOT(saveFile(bool)));
    connect(ui->action_SaveAs,SIGNAL(triggered(bool)),this,SLOT(saveAsFile()));

    //---------------------------编辑部分----------------------------------
    connect(ui->action_Undo,SIGNAL(triggered(bool)),this,SLOT(undo()));
    connect(ui->action_Redo,SIGNAL(triggered(bool)),this,SLOT(redo()));
    connect(ui->action_AllSelect,SIGNAL(triggered(bool)),this,SLOT(allSelect()));
    connect(ui->action_Cut,SIGNAL(triggered(bool)),this,SLOT(cut()));
    connect(ui->action_Copy,SIGNAL(triggered(bool)),this,SLOT(copy()));
    connect(ui->action_Paste,SIGNAL(triggered(bool)),this,SLOT(paste()));
    connect(ui->action_zhushi,SIGNAL(triggered(bool)),this,SLOT(Comment()));
    connect(ui->action_Find,SIGNAL(triggered(bool)),this,SLOT(showFind()));
    connect(ui->action_Replace,SIGNAL(triggered(bool)),this,SLOT(showReplace()));

    connect(&find,SIGNAL(findLetter(QString,bool,bool)),this,SLOT(pushFindLetter(QString,bool,bool)));
    //connect(&find,SIGNAL(findLetter(QString,bool,bool)),this,SLOT(pushFindLetter(QString,bool,bool)));
    connect(&find,SIGNAL(replaceSelect(QString,QString,bool,bool,bool)),this,SLOT(pushReplaceSelect(QString,QString,bool,bool,bool)));


    //---------------------------编译部分-----------------------------------
    connect(ui->action_Run,SIGNAL(triggered(bool)),this,SLOT(comp()));
    connect(ui->action_Run_2,SIGNAL(triggered(bool)),this,SLOT(run()));
    connect(ui->plainTextEdit,SIGNAL(textChanged()),this,SLOT(on_changed()));//当文本内容发生变化时，触发on_changed函数

    //关闭分页
    connect(codeeditor->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(removeSubTab(int)));
    //this->grabKeyboard();
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

//--------新建文件----------
//1.新建主窗口对象
//2.确定新窗口位置
//3.新建窗口
//---------LCH+YSY------------
void MainWindow::newFile()
{
    qDebug()<<"新建";
    QWidget *widget = new QWidget();
    QVBoxLayout *v = new QVBoxLayout();
    CodeEditor c;
    v->addWidget(c.geteditor());
    v->addWidget(c.getconsole());
    v->setContentsMargins(0,0,0,0);
    v->setStretchFactor(c.geteditor(), 4);
    v->setStretchFactor(c.getconsole(), 1);
    widget->setLayout(v);
    codeeditor->tabWidget->addTab(widget,"untitled.cpp");
    codeeditor->tabWidget->setCurrentWidget(widget);
}

//-------------打开文件-------------------
void MainWindow::openFile()
{
    //重写的打开文件
    QString path = QFileDialog::getOpenFileName(this,tr("打开文件"),".",tr("C++ source Files(*.cpp *.c *.h)"));
    if(!path.isEmpty())
    {
        QFile file(path);
        if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
            QMessageBox::warning(this,tr("错误"),tr("打开文件失败"));
            return ;
        }

        int index = codeeditor->tabWidget->currentIndex();
        QRegularExpression re(tr("(?<=\\/)\\w+\\.cpp|(?<=\\/)\\w+\\.c|(?<=\\/)\\w+\\.h"));
        File[index] = re.match(path).captured();
        QTextStream in(&file);
        QWidget *widget = codeeditor->tabWidget->currentWidget();
        QList<QsciScintilla*> c = widget->findChildren<QsciScintilla *>();
        QsciScintilla *e = c.at(0);
        e->setText(in.readAll());
        file.close();
        codeeditor->tabWidget->setTabText(index,File[index]);
    }
    else
    {
        return ;
    }
}

//----------保存文件------------
void MainWindow::saveFile(bool flag)
{
    //重写的保存文件
    int index = codeeditor->tabWidget->currentIndex();
    //该窗口第一次保存
    if(File[index].isEmpty())
    {
        QString path = QFileDialog::getSaveFileName(this,tr("Open File"),fileName,tr("C++ Source File(*.cpp *.c *.h)"));
        if(!path.isEmpty())//选择好了路径
        {
            QFile file(path);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::information(this,"保存文件",tr("保存文件失败!"));
                return ;
            }

            if(flag == false)
            {
                QMessageBox::information(this,"保存文件",tr("保存文件成功!"));
            }
            QRegularExpression re(tr("(?<=\\/)\\w+\\.cpp|(?<=\\/)\\w+\\.c|(?<=\\/)\\w+\\.h"));
            File[index] = re.match(path).captured();
            QTextStream out(&file);
            QWidget *widget = codeeditor->tabWidget->currentWidget();
            QList<QsciScintilla*> c = widget->findChildren<QsciScintilla *>();
            QsciScintilla *e = c.at(0);
            QString str = e->text();
            qDebug()<<str;
            out<<str;
            file.close();
            codeeditor->tabWidget->setTabText(index,File[index]);
        }
        else//取消了保存
        {
            if(flag == false)
            {
                QMessageBox::information(this,tr("保存文件"),tr("您取消了保存!"));
            }
        }
    }
    else//该窗口已经保存过
    {
        FILE *p = fopen(File[index].toStdString().data(),"w");
        if(p==NULL)
            return ;
        QWidget *widget = codeeditor->tabWidget->currentWidget();
        QList<QsciScintilla*> c = widget->findChildren<QsciScintilla *>();
        QsciScintilla *e = c.at(0);
        QString str = e->text();
        fputs(str.toStdString().data(),p);
        if(flag == false)
        {
            QMessageBox::information(this,"保存文件",tr("保存文件成功!"));
        }
        fclose(p);
    }
}

//-------另存为------------
//1.获取文件名
//2.判断文件名是否为空
//3.保存过程同上
//--------LCH------------
void MainWindow::saveAsFile()
{
    int index = codeeditor->tabWidget->currentIndex();
    QString path = QFileDialog::getSaveFileName(this,tr("Open File"),fileName,tr("C++ Source File(*.cpp *.c *.h)"));
    if(!path.isEmpty())//选择好了路径
    {
        QFile file(path);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::information(this,"保存文件",tr("保存文件失败!"));
            return ;
        }

        QMessageBox::information(this,"另存为",tr("另存为文件成功!"));
        QRegularExpression re(tr("(?<=\\/)\\w+\\.cpp|(?<=\\/)\\w+\\.c|(?<=\\/)\\w+\\.h"));
        File[index] = re.match(path).captured();
        QTextStream out(&file);
        QWidget *widget = codeeditor->tabWidget->currentWidget();
        QList<QsciScintilla*> c = widget->findChildren<QsciScintilla *>();
        QsciScintilla *e = c.at(0);
        QString str = e->text();
        qDebug()<<str;
        out<<str;
        file.close();
        codeeditor->tabWidget->setTabText(index,File[index]);
    }
    else
    {
        return ;
    }
}

//多行注释
void MainWindow::Comment()
{
    qDebug()<<"注释";

    QWidget *widget = codeeditor->tabWidget->currentWidget();
    QList<QsciScintilla*> c = widget->findChildren<QsciScintilla *>();
    QsciScintilla *e = c.at(0);

    e->getSelection(&from,&start,&to,&end);
    qDebug()<<from<<"+"<<to<<"+"<<start<<"+"<<end<<endl;
    if(from>to){
        temp=from;
        from=to;
        to=temp;
    }
    int flag = 1;
    for(i=from;i<=to;i++){
        if(e->wordAtLineIndex(i,0)!=""){
            flag=0;
            break;
        }
    }
    qDebug()<<"flag="<<flag;
    if(!flag){
        for(int i=from;i<=to;i++){
            e->insertAt(tr("//"),i,0);
        }
    }
    else {
        for(int i=from;i<=to;i++){
            e->setSelection(i,0,i,2);
            e->removeSelectedText();
        }
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
//替换
void MainWindow::showReplace()
{
    //this->releaseKeyboard();
    qDebug()<<"替换界面出现";
    find.show();
}
//查找
void MainWindow::showFind()
{
    //this->releaseKeyboard();
    qDebug()<<"查找界面出现";
    find.show();
}

void MainWindow::pushFindLetter(QString letter, bool match, bool forward)
{
    qDebug()<<"letter="<<letter;
    QWidget *widget = codeeditor->tabWidget->currentWidget();
    QList<QsciScintilla*> c = widget->findChildren<QsciScintilla *>();
    QsciScintilla *e = c.at(0);

    if(!e->findFirst(letter,false,match,false,true,forward)){
        QMessageBox msg(NULL);
        msg.setWindowTitle("Find");
        msg.setText("Can not find \""+letter+"\"");
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);

        msg.setWindowFlags(Qt::WindowStaysOnBottomHint);
        msg.exec();
    }
}

void MainWindow::pushReplaceSelect(QString letter, QString replaceTo, bool match, bool forward, bool replaceall)
{
    qDebug()<<"letter="<<letter;
    qDebug()<<"replaceto="<<replaceTo;
    QWidget *widget = codeeditor->tabWidget->currentWidget();
    QList<QsciScintilla*> c = widget->findChildren<QsciScintilla *>();
    QsciScintilla *e = c.at(0);
    if(!e->hasSelectedText())
        pushFindLetter(letter,match,forward);

    if(!replaceall){
        e->replace(replaceTo);
        pushFindLetter(letter,match,forward);
    }
    else{
        while(e->findFirst(letter,false,match,false,true,forward))
            e->replace(replaceTo);
        QMessageBox msg(NULL);
        msg.setWindowTitle("Replace");
        msg.setText("Finished");
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setWindowFlags(Qt::WindowStaysOnTopHint);
        msg.exec();
    }
}
//BUG 编译部分有个bug，把文件放到build-IDE文件夹里才能编译

void MainWindow::precomp()//预编译
{
    int index = codeeditor->tabWidget->currentIndex();
    QString FileName = File[index];
    FILE *p = fopen(FileName.toStdString().data(),"r");
    if(p == NULL) return ;
    QString cmd = FileName +".c";
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
    saveFile(true);
    precomp();//自动以预编译

    int index = codeeditor->tabWidget->currentIndex();
    QString FileName = File[index];
    const char *s = FileName.toStdString().data();
    QString cmd;
    cmd.sprintf("gcc -o %s.exe %s.c 2> err.txt",s,s);
    system(cmd.toStdString().data());//先编译

    cmd = FileName.replace("/","\\") + ".c";
    remove(cmd.toStdString().data());

    FILE *f = fopen("err.txt","r");
    QString str = "";
    while(!feof(f))
    {
        char buf[1024] = {0};
        fgets(buf,sizeof(buf),f);
        str += buf;
    }
    if(str=="")
    {
        QWidget *widget = codeeditor->tabWidget->currentWidget();
        QList<QPlainTextEdit*> c = widget->findChildren<QPlainTextEdit *>();
        QPlainTextEdit *e = c.at(0);
        e->clear();
        e->setPlainText("编译成功");
    }
    else
    {
        QWidget *widget = codeeditor->tabWidget->currentWidget();
        QList<QPlainTextEdit*> c = widget->findChildren<QPlainTextEdit *>();
        QPlainTextEdit *e = c.at(0);
        e->clear();
        e->setPlainText(str);
    }
}

void MainWindow::run()
{
    int index = codeeditor->tabWidget->currentIndex();
    QString FileName = File[index];
    QString cmd;
    cmd = FileName + ".exe";
    system(cmd.toStdString().data());

    remove(cmd.toStdString().data());
}

void MainWindow::removeSubTab(int index)//关闭分页
{
    QWidget *widget = codeeditor->tabWidget->currentWidget();
    QList<QsciScintilla*> c = widget->findChildren<QsciScintilla *>();
    QsciScintilla *e = c.at(0);
    QString editortext = e->text();
    if(File[index].isEmpty())//如果没有保存过
    {
        if(editortext=="")//如果没有修改过
        {
            codeeditor->tabWidget->removeTab(index);
        }
        else
        {
            QString dlgTitle="Question消息框";
            QString strInfo="文件已被修改，是否保存修改？";

            QMessageBox::StandardButton  defaultBtn=QMessageBox::NoButton; //缺省按钮

            QMessageBox::StandardButton result;//返回选择的按钮
            result=QMessageBox::question(this, dlgTitle, strInfo,
                                         QMessageBox::Yes|QMessageBox::No |QMessageBox::Cancel,
                                         defaultBtn);

            if (result==QMessageBox::Yes)//保存修改
            {
                saveFile(true);
                codeeditor->tabWidget->removeTab(index);
            }
            else if(result==QMessageBox::No)//不保存修改
            {
                codeeditor->tabWidget->removeTab(index);
            }
        }
    }
    else//如果保存过
    {
        FILE *p = fopen(File[index].toStdString().data(),"r");
        QString filetext;
        while(!feof(p))
        {
            char buf[1024] = {0};
            fgets(buf,sizeof(buf),p);
            filetext += buf;
        }
        if(editortext == filetext)//如果没有修改过
        {
            codeeditor->tabWidget->removeTab(index);
        }
        else
        {
            QString dlgTitle="Question消息框";
            QString strInfo="文件已被修改，是否保存修改？";

            QMessageBox::StandardButton  defaultBtn=QMessageBox::NoButton; //缺省按钮

            QMessageBox::StandardButton result;//返回选择的按钮
            result=QMessageBox::question(this, dlgTitle, strInfo,
                                         QMessageBox::Yes|QMessageBox::No |QMessageBox::Cancel,
                                         defaultBtn);

            if (result==QMessageBox::Yes)//保存修改
            {
                saveFile(true);
                codeeditor->tabWidget->removeTab(index);
            }
            else if(result==QMessageBox::No)//不保存修改
            {
                codeeditor->tabWidget->removeTab(index);
            }
        }
    }
    File[index].clear();
}

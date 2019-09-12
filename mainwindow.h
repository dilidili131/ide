#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qtextedit.h>
#include "codeeditor.h"
#include "find.h"
#include "tree.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    CodeEditor *codeeditor = new CodeEditor(this);

    QString fileName;
    QString File[1024];
    QString path;
    //bool isSaved;
    //bool isRunning;
    //bool Flag_isOpen = false;       //标记：判断是否打开或创建了一个文件
    //bool Flag_isNew = true;       //教训：初始为true，次应用而为C语言ide，打开不用判断文件是否新建，可直接写代码

    void initFileData();//初始化文件信息
    void precomp();//预编译
    //注释
    int from;
    int to;
    int i;
    int temp;
    int start;
    int end;
    Find find;//查找


    //槽函数链接
public slots:
    //----------文件部分---------
    void newFile();    //新建文件
    void openFile();   //打开文件
    void saveFile(bool flag);   //保存文件
    void saveAsFile(); //另存为
    void treeOpenFile(QModelIndex);

    //----------编辑部分---------
    void undo();       //撤销
    void redo();       //重做
    void allSelect();  //全选
    void cut();        //剪切
    void copy();       //复制
    void paste();      //粘贴
    void Comment();    //注释
    void showReplace();//替换
    void showFind();   //查找
    //void AnnotationHiden(); //注释隐藏
    void pushFindLetter(QString, bool, bool);
    void pushReplaceSelect(QString, QString, bool, bool, bool);

    //---------编译部分----------
    void comp();
    void run();
    void removeSubTab(int index);
    //----------帮助------------
    void about();   //关于我们
    void description();    //说名文档



protected:
    bool eventFilter(QObject *obj, QEvent *event);
private slots:
};

#endif // MAINWINDOW_H

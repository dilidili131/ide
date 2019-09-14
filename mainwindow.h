#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qtextedit.h>
#include "codeeditor.h"
#include "find.h"
#include "debug.h"
//#include "tree.h"
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

    Debug debug;

    //用于注释隐藏
    bool isAnnotationHide;
    struct antt{
        int pos;
        QString an;
    };
    std::vector<antt> annotate;



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

    void recordPos();//记录位置
    void annotate_hide_and_show();//槽函数


    //---------编译部分----------
    void comp();
    void run();
    void removeSubTab(int index);
    void pushDebug();
    //----------帮助------------
    void about();   //关于我们
    void description();    //说名文档



protected:
    bool eventFilter(QObject *obj, QEvent *event);
private slots:
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qtextedit.h>
#include "codeeditor.h"

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
    bool isSaved;
    bool isRunning;
    bool isChanged;
    bool Flag_isOpen = false;       //标记：判断是否打开或创建了一个文件
    bool Flag_isNew = true;        //教训：初始为true，次应用而为C语言ide，打开不用判断文件是否新建，可直接写代码
    QString Last_FileName;     //最后一次保存的文件的名字
    QString Last_FileContent;  //最后一次保存文件的内容

    void initFileData();//初始化文件信息
    void precomp();//预编译

    //槽函数链接
public slots:
    //----------文件部分---------
    void newFile();    //新建文件
    void openFile();   //打开文件
    void saveFile();   //保存文件
    void saveAsFile(); //另存为
    void saveWarn(QCloseEvent *event);   //保存提醒

    //----------编辑部分---------
    void undo();       //撤销
    void redo();       //重做
    void allSelect();  //全选
    void cut();        //剪切
    void copy();       //复制
    void paste();      //粘贴

    //---------编译部分----------
    void comp();
    void run();
    void onChanged();
    void removeSubTab(int index);


private slots:
};

#endif // MAINWINDOW_H

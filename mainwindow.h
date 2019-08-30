#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qtextedit.h>

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
    QString fileName;
    QString filePath;\
    bool isSaved;
    bool isRunning;

    void initFileData();//初始化文件信息

//槽函数链接
public slots:
    //----------文件部分---------
    void newFile();    //新建文件
    void openFile();   //打开文件
    void saveFile();   //保存文件

    //----------编辑部分---------
    void undo();       //撤销
    void redo();       //重做
    void allSelect();  //全选
    void cut();        //剪切
    void copy();       //复制
    void paste();      //粘贴

private slots:
    void on_textEdit_destroyed();
};

#endif // MAINWINDOW_H

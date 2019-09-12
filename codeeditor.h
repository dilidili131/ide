#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include "tree.h"

#include <QWidget>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qscilexerhtml.h>
#include <QPlainTextEdit>
#include <QTabWidget>
#include <QTreeView>
class CodeEditor : public QWidget
{
    Q_OBJECT
public:
    CodeEditor(QWidget *parent = 0);

    QsciScintilla *geteditor(){ //返回QScintilla的对象指针
        return editor;
    }
    QPlainTextEdit *getconsole(){ //返回QScintilla的对象指针
        return console;
    }
    QTabWidget *tabWidget;
    QTreeView *treeview;
    QSplitter *splitter;
    QDirModel *model ;


private:
    //文件树


    QsciScintilla *editor=new QsciScintilla(this);
    QPlainTextEdit *console = new QPlainTextEdit(this);

signals:


public slots:

protected:
    //void keyPressEvent(QKeyEvent *event) override;
};

#endif // CODEEDITOR_H

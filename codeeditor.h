#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QWidget>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qscilexerhtml.h>

class CodeEditor : public QWidget
{
    Q_OBJECT
public:
    CodeEditor(QWidget *parent = 0);

    QsciScintilla *geteditor(){ //返回QScintilla的对象指针
        return editor;
    }
    QsciScintilla *getconsole(){ //返回QScintilla的对象指针
        return console;
    }

private:
    QsciScintilla *editor=new QsciScintilla(this);
    QsciScintilla *console=new QsciScintilla(this);

signals:

public slots:
};

#endif // CODEEDITOR_H

#include "codeeditor.h"
#include "mainwindow.h"
#include "qmainwindow.h"
#include <QVBoxLayout>
#include <Qsci/qsciapis.h>
#include <QPushButton>

#include <QApplication>
#include <QVBoxLayout>
#include <qmessagebox.h>

#include <iostream>
CodeEditor::CodeEditor(QWidget *parent)
    : QWidget(parent)
{

    //行号提示
    editor->setMarginType(0,QsciScintilla::NumberMargin);//设置编号为0的页边显示行号。
    editor->setMarginLineNumbers(0,true);//对该页边启用行号
    editor->setMarginWidth(0,30);//设置页边宽度

    //界面
    tabWidget = new QTabWidget();
    QVBoxLayout *pLayout = new QVBoxLayout();
    QWidget *widget1 = new QWidget();
    pLayout->addWidget(editor);
    pLayout->addWidget(console);
    pLayout->setContentsMargins(0,0,0,0);
    pLayout->setStretchFactor(editor, 4);
    pLayout->setStretchFactor(console, 1);
    widget1->setLayout(pLayout);
    //const char *s = fileName.toStdString().data();
    tabWidget->addTab(widget1,"untitled.cpp");

    tabWidget->setTabsClosable(true);//添加关闭按钮
    //connect(tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(removeSubTab(int)));

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(tabWidget);
    this->setLayout(layout);

    //设置显示字体
    editor->setFont(QFont("Courier New"));
    //设置编码方式
    editor->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CP_UTF8);//设置编码为UTF-8

    //高亮
    QsciLexer *textLexer;
    textLexer = new QsciLexerCPP;
    editor->setLexer(textLexer);//给QsciScintilla设置词法分析器

    //代码提示
    QsciAPIs *apis = new QsciAPIs(textLexer);
    if(!apis->load(QString(":/buquan/api.txt"))) {
        QMessageBox::warning(this,QString("提示"),QString("读取文件失败"));
    }
    else
        apis->prepare();

    editor->setAutoCompletionSource(QsciScintilla::AcsAll);   //设置源，自动补全所有地方出现的
    editor->setAutoCompletionCaseSensitivity(true);   //设置自动补全大小写敏感
    editor->setAutoCompletionThreshold(2);    //设置每输入2个字符就会出现自动补全的提示
    //editor->setAutoCompletionReplaceWord(true);

    //设置自动缩进
    editor->setAutoIndent(true);

    //显示选中行号
    editor->setCaretLineVisible(true);
    editor->setCaretLineBackgroundColor(Qt::lightGray);

    //Enables or disables, according to enable, this display of indentation guides.
    editor->setIndentationGuides(true);

    //显示行号背景颜色
    //editor->setMarginsBackgroundColor(Qt::gray);

    //It is ignored if an indicator is being used. The default is blue.
    editor->setUnmatchedBraceForegroundColor(Qt::blue);

    //括号匹配
    editor->setBraceMatching(QsciScintilla::SloppyBraceMatch);
    //this->grabKeyboard();

    //代码折叠
    editor->setMarginType(3,QsciScintilla::SymbolMargin);
    editor->setMarginLineNumbers(3,false);
    editor->setMarginWidth(3,15);
    editor->setMarginSensitivity(3,true);
    editor->setFolding(QsciScintilla::BoxedTreeFoldStyle,3);
}

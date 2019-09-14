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
    treeview = new QTreeView();
    model =new  QDirModel;
    treeview->setModel(model);
    treeview->setRootIndex(model->index("c:\\"));
    //treeview->setColumnWidth(0,100);
    treeview->hideColumn(1);
    treeview->hideColumn(2);
    treeview->hideColumn(3);

    //行号提示
    editor->setMarginType(0,QsciScintilla::NumberMargin);//设置编号为0的页边显示行号。
    editor->setMarginLineNumbers(0,true);//对该页边启用行号
    editor->setMarginWidth(0,30);//设置页边宽度

    //界面
    QHBoxLayout *hlayout = new  QHBoxLayout();
    hlayout->setContentsMargins(0,0,0,0);
    QVBoxLayout *vlayout1 = new QVBoxLayout();//目录
    vlayout1->addWidget(treeview);
    QVBoxLayout *vlayout2 = new QVBoxLayout();//code
    vlayout2->addWidget(editor);
    vlayout2->addWidget(console);
    vlayout2->setStretchFactor(editor,4);
    vlayout2->setStretchFactor(console,1);

    hlayout->addLayout(vlayout1);
    hlayout->addLayout(vlayout2);
    hlayout->setStretchFactor(vlayout1,1);
    hlayout->setStretchFactor(vlayout2,6);

    tabWidget = new QTabWidget();
    QWidget *widget1 = new QWidget();
    widget1->setLayout(hlayout);

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


    //代码折叠
    //    editor->setMarginType(3,QsciScintilla::SymbolMargin);
    //    editor->setMarginLineNumbers(3,false);
    //    editor->setMarginWidth(3,15);
    //    editor->setMarginSensitivity(3,true);
    editor->setFolding(QsciScintilla::BoxedTreeFoldStyle,3);

    //断点设置
    editor->setMarginType(1, QsciScintilla::SymbolMargin);
    editor->setMarginLineNumbers(1, false);
    editor->setMarginSensitivity(1,true);
    connect(editor, SIGNAL(marginClicked(int, int, Qt::KeyboardModifiers)),this,
            SLOT(on_margin_clicked(int, int, Qt::KeyboardModifiers)));

    editor->markerDefine(QsciScintilla::Circle, 1);
    editor->setMarkerBackgroundColor(QColor("#ee1111"), 1);

}
//有bug，新建页面不能添加断点
//换行marker会跑
void CodeEditor::on_margin_clicked(int margin, int line, Qt::KeyboardModifiers)
{
    qDebug("%d %d",margin,line);
    if (margin == 1) {
            if (editor->markersAtLine(line) != 0) {
                editor->markerDelete(line, 1);
            }
            else {
                editor->markerAdd(line, 1);
            }
        }
}





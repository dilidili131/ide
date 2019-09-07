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
    // apis->add(QString("import"));
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
    this->grabKeyboard();


    //TODO 查找替换


    //TODO 代码折叠


    //TODO 注释

}
//括号补全
void CodeEditor::keyPressEvent(QKeyEvent *event)
{
    int hang,lie;
    editor->getCursorPosition(&hang,&lie);
    if(event->key()==16777235)//up
    {
        editor->setCursorPosition(hang-1,lie);
    }
    else if(event->key()==16777237)//down
    {
        editor->setCursorPosition(hang+1,lie);
    }
    else   if(event->key()==16777234)//left
    {
        editor->setCursorPosition(hang,lie-1);
    }
    else if(event->key()==16777236)//right
    {
        editor->setCursorPosition(hang,lie+1);
    }
    else if(event->key()==16777219)//退格
    {
        if(editor->selectedText()=="")
        {
            if(lie==0)//删除整行
            {
                editor->setSelection(hang,lie,hang-1,lie);
                QString tmp=editor->selectedText();
                tmp=tmp.left(tmp.length()-1);
                editor->replaceSelectedText(tmp);
            }
            else {
                editor->setSelection(hang,lie,hang,lie-1);
                editor->removeSelectedText();
            }
        }
        else //移到上一行末尾按delete
        {
            editor->removeSelectedText();
        }
    }
    else if(event->key()==16777223)//delete
    {

        if(editor->selectedText()=="")
        {
            editor->setSelection(hang,lie,hang,lie+1);
            editor->removeSelectedText();
        }
        else editor->removeSelectedText();
    }
    else if(event->key()==40)
    {
        editor->insertAt(")",hang,lie);
        editor->insertAt(event->text(),hang,lie);
        editor->setCursorPosition(hang,lie+1);
    }
    else if(event->key()==123)
    {
        editor->insertAt("}",hang,lie);
        editor->insertAt(event->text(),hang,lie);
        editor->setCursorPosition(hang,lie+1);
        this->releaseKeyboard();
        QKeyEvent *leftKey = new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
        QCoreApplication::postEvent(editor, leftKey);
        this->grabKeyboard();
    }
    else if(event->key()==91)
    {
        editor->insertAt("]",hang,lie);
        editor->insertAt(event->text(),hang,lie);
        editor->setCursorPosition(hang,lie+1);
    }
    else if(event->key()==60)
    {
        editor->insertAt(">",hang,lie);
        editor->insertAt(event->text(),hang,lie);
        editor->setCursorPosition(hang,lie+1);
    }
    else if(event->key()==16777248||event->key()==16777249 ||event->key()==16777252)//送花ift and ctrl DAXIE
    {
    }
    else if(event->key()== 16777220)
    {
        this->releaseKeyboard();
        QKeyEvent *leftKey = new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
        QCoreApplication::postEvent(editor, leftKey);
        this->grabKeyboard();
    }
    else //all
    {
        editor->insertAt(event->text(),hang,lie);
        editor->setCursorPosition(hang,lie+1);
    }
    std::cout<<event->key()<<' '<<hang<<' '<<lie<<std::endl;
}




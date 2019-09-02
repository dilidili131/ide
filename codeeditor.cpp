#include "codeeditor.h"
#include "codeeditor.h"

#include <Qsci/qsciapis.h>

#include <QVBoxLayout>
CodeEditor::CodeEditor(QWidget *parent)
    : QWidget(parent)
{

    //行号提示
    editor->setMarginType(0,QsciScintilla::NumberMargin);//设置编号为0的页边显示行号。
    editor->setMarginLineNumbers(0,true);//对该页边启用行号
    editor->setMarginWidth(0,15);//设置页边宽度

    //界面
    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->addWidget(editor);
    pLayout->setContentsMargins(0,0,0,0);

    //设置显示字体
    editor->setFont(QFont("Courier 10 Pitch"));
    //设置编码方式
    editor->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CP_UTF8);//设置编码为UTF-8

    QsciLexer *textLexer;
    textLexer = new QsciLexerCPP;

    editor->setLexer(textLexer);//给QsciScintilla设置词法分析器

    //代码提示
    QsciAPIs *apis = new QsciAPIs(textLexer);

    apis->prepare();

    editor->setAutoCompletionSource(QsciScintilla::AcsAll);   //设置源，自动补全所有地方出现的
    editor->setAutoCompletionCaseSensitivity(true);   //设置自动补全大小写敏感
    editor->setAutoCompletionThreshold(1);    //设置每输入2个字符就会出现自动补全的提示
    //editor->setAutoCompletionReplaceWord(true);   //when an item from an auto-completion list is selected, the rest of the word to the right of the current cursor is removed.


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

}



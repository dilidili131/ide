#include "find.h"
#include "ui_find.h"
#include <QDebug>
Find::Find(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Find)
{
    ui->setupUi(this);
    ui->forward->setChecked(true);
    connect(ui->findnext,SIGNAL(clicked),this,SLOT(on_findnext_clicked()));
    connect(ui->replace,SIGNAL(clicked),this,SLOT(on_replace_clicked()));
    connect(ui->replaceall,SIGNAL(clicked),this,SLOT(on_replaceall_clicked()));

    //this->grabKeyboard();
}

Find::~Find()
{
    delete ui;
}

void Find::on_findnext_clicked() //查找
{
    qDebug()<<"查找button";
    QString letter = ui->lineEdit_find->text();
    match = ui->match->isChecked();
    forward = ui->forward->isChecked();
    emit findLetter(letter,match,forward);
}

void Find::on_replace_clicked() //替换
{
    qDebug()<<"替换button";
    QString letter_find = ui->lineEdit_find->text();
    QString letter_repalce = ui->lineEdit_replace->text();
    match = ui->match->isChecked();
    forward = ui->forward->isChecked();
    emit replaceSelect(letter_find,letter_repalce,match,forward,false);
}

void Find::on_replaceall_clicked() //全部替换
{
    qDebug()<<"全部替换button";
    QString letter_find = ui->lineEdit_find->text();
    QString letter_repalce = ui->lineEdit_replace->text();
    emit replaceSelect(letter_find,letter_repalce,match,forward,true);
}

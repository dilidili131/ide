#include "debug.h"
#include "ui_debug.h"


Debug::Debug(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Debug)
{
    ui->setupUi(this);
    connect(ui->onDebug, SIGNAL(clicked()),this,SLOT(onDebug()));
    connect(ui->onCon, SIGNAL(clicked()),this,SLOT(onContinue()));
    connect(ui->onRun, SIGNAL(clicked()),this,SLOT(onRun()));
    connect(ui->onNext, SIGNAL(clicked()),this,SLOT(next()));
    connect(ui->addWatch, SIGNAL(clicked()),this,SLOT(addVar()));
    connect(ui->onStep, SIGNAL(clicked()),this,SLOT(step()));
    ui->onCon->setEnabled(false);
    ui->onRun->setEnabled(false);
    ui->onNext->setEnabled(false);
    ui->addWatch->setEnabled(false);
    ui->onStep->setEnabled(false);
    ui->bugMes->setText("");
}

Debug::~Debug()
{
    ui->bugMes->setText("");
    delete ui;
}

void Debug::onDebug()
{
    QStringList list = fname.split(".");
    const char *n = fname.toStdString().data();
    const char *s = list[0].toStdString().data();
    QString cmd;
    cmd.sprintf("gcc -o %s.exe %s -g",s,n);
    system(cmd.toStdString().data());


    process = new QProcess();
    connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(read_slot()));
    connect(process,SIGNAL(readyReadStandardError()),this,SLOT(read_slot()));

    //启动调试
    cmd.sprintf("gdb %s --silent",s);//<a.bmp>a.txt
    process->start(cmd.toStdString().data());
    ui->bugMes->append("<font color=green>----ready----</font>");
    ui->onCon->setEnabled(true);
    ui->onRun->setEnabled(true);
    ui->onNext->setEnabled(true);
    ui->addWatch->setEnabled(true);
    ui->onStep->setEnabled(true);

    //breaks.push_back(20);
    addBreak();

}
void Debug::onContinue()
{
    ui->bugMes->append("<font color=green>----continue----</font>");
    QString cmd;
    cmd = "continue \r\n";
    process->write(cmd.toStdString().data());
    process->waitForFinished(500);
    qDebug("continue");
}

void Debug::onRun()
{
    ui->bugMes->append("<font color=green>----run----</font>");
    QString cmd;
    cmd = "run \r\n";
    process->write(cmd.toStdString().data());
    process->waitForFinished(500);
    showVar();
    qDebug("run");
}
//断点必须在启动debug窗口之前提交
void Debug::addBreak()
{
    for(std::vector<int>::iterator it = breaks.begin(); it != breaks.end(); it++)
    {
        qDebug() << "<<<" << QString::number(*it);
        QString cmd;
        cmd = "break " + QString::number(*it) + "\r\n";//+ fname + ":"
        process->write(cmd.toStdString().data());
        process->waitForFinished(500);
    }
    ui->bugMes->append("<font color=green>Add breakpoints</font>");

}

void Debug::addVar()
{
    QString varrr;
    varrr = ui->varEdit->text();
    var.push_back(varrr);
    ui->varEdit->setText("");
    ui->bugMes->append("<font color=green>Add watch </font>" + varrr);
//    QString cmd;
//    cmd = "display " + varrr + "\r\n";
//    qDebug() << cmd;
}

void Debug::showVar()
{
    for(std::vector<QString>::iterator it = var.begin(); it != var.end(); it++)
    {
        ui->bugMes->append((*it) + ":");
        QString cmd;
        cmd = "p " + (*it) + "\r\n";
        process->write(cmd.toStdString().data());
        process->waitForFinished(500);
    }
}

void Debug::next()
{
    ui->bugMes->append("<font color=green>----next----</font>");
    QString cmd;
    cmd = "n \r\n";
    process->write(cmd.toStdString().data());
    process->waitForFinished(500);
    showVar();
    qDebug("run");
}

void Debug::step()
{
    ui->bugMes->append("<font color=green>----step----</font>");
    QString cmd;
    cmd = "s \r\n";
    process->write(cmd.toStdString().data());
    process->waitForFinished(500);
    showVar();
    qDebug("run");
}

void Debug::read_slot()
{
    while(process->canReadLine())
    {
        char output[1024];
        long long ret = process->readLine(output,1023);
        qDebug() << "<<<<<<<<<<" << output;

        QString tmp;
        tmp = output;
        ui->bugMes->append(tmp);
        if(tmp.indexOf("exited normally]") > -1 || tmp.indexOf("__register_frame_info") > -1)
        {
            process->kill();
            ui->bugMes->append("<font color=green>-exited normally-</font>");
            quitBugging();
        }

        //接收到error信号
        if(tmp.indexOf("Error") > -1)
        {
            process->kill();
            ui->bugMes->append("<font color=red>----error----</font>");
            quitBugging();
        }
    }
}

void Debug::quitBugging()
{
    QString cmd;
    cmd = "q \r\n";
    process->write(cmd.toStdString().data());
    process->waitForFinished(500);
    ui->onCon->setEnabled(false);
    ui->onRun->setEnabled(false);
    ui->onNext->setEnabled(false);
    ui->addWatch->setEnabled(false);
}

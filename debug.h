#ifndef DEBUG_H
#define DEBUG_H

#include <QPushButton>
#include <QDialog>
#include <QThread>
#include <QString>
#include <QProcess>
#include <QDebug>

namespace Ui{
class Debug;
}

class Debug : public QDialog
{
    Q_OBJECT

public:
    explicit Debug(QWidget *parent = nullptr);
    ~Debug();
    QString fname;
    std::vector<int> breaks;

private slots:
    void onDebug();
    void read_slot();
    void addBreak();
    void addVar();
    void showVar();
    void onContinue();
    void onRun();
    void quitBugging();
    void next();
    void step();

signals:

private:
    Ui::Debug *ui;
    QProcess *process;
    std::vector<QString> var;

protected:

};

#endif // DEBUG_H

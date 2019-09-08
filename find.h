#ifndef FIND_H
#define FIND_H

#include <QDialog>

namespace Ui {
class Find;
}

class Find : public QDialog
{
    Q_OBJECT

public:
    explicit Find(QWidget *parent = nullptr);
    ~Find();


private slots:
    void on_findnext_clicked();   //查找
    void on_replace_clicked();  //替换
    void on_replaceall_clicked(); //全部替换

signals:
    void findLetter(QString, bool, bool);
    void replaceSelect(QString, QString, bool, bool, bool);

private:
    Ui::Find *ui;
protected:
    bool match;
    bool forward;
};

#endif // FIND_H

#ifndef ADDMUSICDIALOG_H
#define ADDMUSICDIALOG_H

#include <QDialog>
#include<QFileDialog>
#include<QMessageBox>
#include"mainwindow.h"
namespace Ui {
class addmusicDialog;
}

class addmusicDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addmusicDialog(QWidget *parent = nullptr);
    ~addmusicDialog();

private slots:
    void on_getPathBtn_clicked();

    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::addmusicDialog *ui;
};

#endif // ADDMUSICDIALOG_H

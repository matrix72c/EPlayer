#ifndef EDITLISTDIALOG_H
#define EDITLISTDIALOG_H

#include <QDialog>
#include<QFileDialog>
#include<QString>
#include<QImage>
#include"mainwindow.h"
namespace Ui {
class editlistDialog;
}

class editlistDialog : public QDialog
{
    Q_OBJECT

public:
    explicit editlistDialog(QWidget *parent = nullptr);
    ~editlistDialog();

private slots:
    void on_fileBtn_clicked();


    void on_delBtn_stateChanged(int arg1);

    void on_okBtn_clicked();

private:
    Ui::editlistDialog *ui;
    QString imgUrl;
};

#endif // EDITLISTDIALOG_H

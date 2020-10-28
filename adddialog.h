#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>

namespace Ui {
class addDialog;
}

class addDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addDialog(QWidget *parent = nullptr);
    ~addDialog();

private slots:
    void on_okBtn_clicked();

private:
    Ui::addDialog *ui;
};

#endif // ADDDIALOG_H

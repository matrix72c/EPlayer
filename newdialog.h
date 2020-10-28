#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>
namespace Ui {
class newDialog;
}

class newDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newDialog(QWidget *parent = nullptr);
    ~newDialog();

private slots:
    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::newDialog *ui;
};

#endif // NEWDIALOG_H

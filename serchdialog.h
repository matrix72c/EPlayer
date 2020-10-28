#ifndef SERCHDIALOG_H
#define SERCHDIALOG_H

#include <QDialog>

namespace Ui {
class serchDialog;
}

class serchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit serchDialog(QWidget *parent = nullptr);
    ~serchDialog();

private:
    Ui::serchDialog *ui;
};

#endif // SERCHDIALOG_H

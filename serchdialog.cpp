#include "serchdialog.h"
#include "ui_serchdialog.h"

serchDialog::serchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::serchDialog)
{
    ui->setupUi(this);
}

serchDialog::~serchDialog()
{
    delete ui;
}

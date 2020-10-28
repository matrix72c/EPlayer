#include "adddialog.h"
#include "ui_adddialog.h"
#include"mainwindow.h"
#include<qmessagebox.h>
addDialog::addDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addDialog)
{
    ui->setupUi(this);
}

addDialog::~addDialog()
{
    delete ui;
}

void addDialog::on_okBtn_clicked()
{

}

#include "newdialog.h"
#include "ui_newdialog.h"
#include"mainwindow.h"
#include<QMessageBox>
newDialog::newDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}

newDialog::~newDialog()
{
    delete ui;
}

void newDialog::on_okBtn_clicked()
{
    QString name = ui->nameEdit->text();
    QString intro = ui->introEdit->text();
    if(name.size()==0)
    {
        QMessageBox *msgBox;
        msgBox = new QMessageBox("错误",
                                 "歌单名字不能为空",
                                 QMessageBox::Critical,
                                 QMessageBox::Ok | QMessageBox::Default,
                                 QMessageBox::Cancel | QMessageBox::Escape,
                                 0);
        msgBox->show();
    }
    else
    {

        QString qsql = "INSERT INTO LIST VALUES(null, '" + name + "','" + intro + "', ':/img/avatar.png')";
        if(intro.size() == 0) qsql = "INSERT INTO LIST VALUES(null, '" + name + "',null, ':/img/avatar.png')";
        MainWindow * p = (MainWindow*) parentWidget();  //获取父窗口指针
        p->db.open();
        QSqlQuery query(p->db);
        query.exec(qsql);
        p->db.close();
        p->reloadList();
        p->listReset(name);
        this->close();
    }
}

void newDialog::on_cancelBtn_clicked()
{
    this->close();
}

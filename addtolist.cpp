#include "addtolist.h"
#include "ui_addtolist.h"
#include"mainwindow.h"
addToList::addToList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addToList)
{
    ui->setupUi(this);
    MainWindow *p = (MainWindow*) parentWidget();  //获取父窗口指针
    p->db.open();
    QSqlQuery query(p->db);
    query.exec("select * from list");
    while(query.next())
    {
        if(query.value(0).toInt() == p->curListId) continue;
        QListWidgetItem *ite = new QListWidgetItem(ui->allLists);
        ite->setText(query.value(1).toString());
        ite->setIcon(QIcon(":/img/ico/playlist_n.png"));
        ite->setWhatsThis(query.value(0).toString());
    }
    p->db.close();
}

addToList::~addToList()
{
    delete ui;
}

void addToList::on_closeBtn_clicked()
{
    this->close();
}

void addToList::on_allLists_itemClicked(QListWidgetItem *item)
{
    int currentRow = item->whatsThis().toInt();
    MainWindow *p = (MainWindow*) parentWidget();  //获取父窗口指针
    p->db.open();
    QSqlQuery query(p->db);
    if(!query.exec("INSERT INTO music_list VALUES(null, " + QString::number(currentRow) + " ," + QString::number(p->musicIdx[p->signalRow]) + ")")) qDebug() << query.lastError();
    this->close();
    p->db.close();
}

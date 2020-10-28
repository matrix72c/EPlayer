#include "searchdialog.h"
#include "ui_searchdialog.h"
#include"mainwindow.h"
searchDialog::searchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::searchDialog)
{

    ui->setupUi(this);
    MainWindow * p = (MainWindow*) parentWidget();  //获取父窗口指针
    p->db.open();
    QSqlQuery query(p->db);
    searchContent = p->searchContent;
    searchContent.prepend("'%");
    searchContent.append("%'");
    QString qsql = "SELECT * FROM music WHERE name LIKE " + searchContent + " OR art LIKE " + searchContent + " OR album LIKE " + searchContent;
    qDebug() << qsql;
    if(!query.exec(qsql)) qDebug() << query.lastError();
    if(!query.first())
    {
        ui->label->setText("什么也没有找到");
        ui->searchNone->setText("这里什么也没有");
    }
    else
    {
        ui->searchNone->close();
        QListWidgetItem *item = new QListWidgetItem(ui->searchList);
        item->setText(query.value(1).toString() + " - " + query.value(2).toString());
        item->setWhatsThis("0");
        ui->searchList->addItem(item);
        infolist.push_back({query.value(0).toString(), query.value(1).toString(), query.value(2).toString(),query.value(3).toString(),query.value(4).toString(),query.value(5).toString()});

        int tmpi = 1;
        while(query.next())
        {
            QListWidgetItem *item = new QListWidgetItem(ui->searchList);
            item->setText(query.value(1).toString() + " - " + query.value(2).toString());
            item->setWhatsThis(QString::number(tmpi));
            ui->searchList->addItem(item);
            infolist.push_back({query.value(0).toString(), query.value(1).toString(), query.value(2).toString(),query.value(3).toString(),query.value(4).toString(),query.value(5).toString()});
            tmpi++;
        }
        ui->label->setText("搜索到共 " + QString::number(tmpi) + " 首歌曲");
    }
    p->db.close();
}

searchDialog::~searchDialog()
{
    delete ui;
}

void searchDialog::on_pushButton_clicked()
{
    this->close();
}



void searchDialog::on_searchList_itemClicked(QListWidgetItem *item)
{
    if(item == NULL) return;
    int currentRow = item->whatsThis().toInt();
    MainWindow * p = (MainWindow*) parentWidget();  //获取父窗口指针
    p->playlist->clear();
    p->playlistInfo.clear();
    p->playlistInfo.push_back(infolist[currentRow]);
    p->playlist->addMedia(QUrl(infolist[currentRow][4]));
    p->player->play();
    this->close();
}

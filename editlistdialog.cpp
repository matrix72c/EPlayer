#include "editlistdialog.h"
#include "ui_editlistdialog.h"

editlistDialog::editlistDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editlistDialog)
{
    ui->setupUi(this);
}

editlistDialog::~editlistDialog()
{
    delete ui;
}

void editlistDialog::on_fileBtn_clicked()
{
    imgUrl = QFileDialog::getOpenFileName(this, "添加封面图片", "D:", "图片文件(*png *jpg *jpeg)");
    ui->avatarPreview->setPixmap(QPixmap(imgUrl).scaled(ui->avatarPreview->width(), ui->avatarPreview->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void editlistDialog::on_delBtn_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        QMessageBox *msgBox;
        msgBox = new QMessageBox("警告",
                                 "你正在进行一项不可逆操作，你确定要删除这个歌单吗",
                                 QMessageBox::Critical,
                                 QMessageBox::Ok | QMessageBox::Default,
                                 QMessageBox::Cancel | QMessageBox::Escape,
                                 0);
        msgBox->show();
    }
}


void editlistDialog::on_okBtn_clicked()
{
    MainWindow * p = (MainWindow*) parentWidget();  //获取父窗口指针
    p->db.open();
    QSqlQuery query(p->db);
    if(ui->delBtn->isChecked())
    {
        if(!query.exec("DELETE FROM list WHERE id = " + QString::number(p->curListId)))
            qDebug() << query.lastError();
        if(!query.exec("DELETE FROM music_list WHERE id = " + QString::number(p->curListId)))
            qDebug() << query.lastError();
        p->reloadList();
        p->listReset("我喜欢的");
    }
    else
    {
        QString name = ui->nameLine->text(), intro = ui->introLine->text();
        if(name.size() == 0)
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
        else if(imgUrl.size() == 0)
        {
            if(!query.exec("UPDATE list SET name = '" + name + "', intro = '" + intro + "' WHERE id = " + QString::number(p->curListId)))
                qDebug() << query.lastError();
            p->reloadList();
            p->listReset(name);
        }
        else
        {
            if(!query.exec("UPDATE list SET name = '" + name + "', intro = '" + intro + "', avatar = '" + imgUrl + "' WHERE id = " + QString::number(p->curListId)))
                qDebug() << query.lastError();
            p->reloadList();
            p->listReset(name);
        }
    }
    p->db.close();
    this->close();
}

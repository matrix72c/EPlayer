#include "addmusicdialog.h"
#include "ui_addmusicdialog.h"

addmusicDialog::addmusicDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addmusicDialog)
{
    ui->setupUi(this);
}

addmusicDialog::~addmusicDialog()
{
    delete ui;
}

void addmusicDialog::on_getPathBtn_clicked()
{
    QString musicUrl = QFileDialog::getOpenFileName(this, "添加一个音频文件", "D:", "音频文件(*mp3 *wav *flac)");
    ui->showPath->setText(musicUrl);
}


void addmusicDialog::on_okBtn_clicked()
{
    if(ui->musicNameEdit->text().size() == 0 || ui->singerEdit->text().size() == 0 || ui->albumEdit->text().size() == 0 || ui->showPath->text().size() == 0)
    {
        QMessageBox *msgBox;
        msgBox = new QMessageBox("错误",
                                 "有信息为空哦~",
                                 QMessageBox::Critical,
                                 QMessageBox::Ok | QMessageBox::Default,
                                 QMessageBox::Cancel | QMessageBox::Escape,
                                 0);
        msgBox->show();
    }
    else
    {
        MainWindow * p = (MainWindow*) parentWidget();  //获取父窗口指针
        p->db.open();
        QSqlQuery query(p->db);
        QString qsql = "SELECT * FROM music WHERE path = '" + ui->showPath->text() +"'";
        query.exec(qsql);
        bool hasmusic = query.first();
        qDebug() << hasmusic;
        qsql = "SELECT * FROM list WHERE id = " + QString::number(p->curListId);
        query.exec(qsql);
        query.first();
        QString curlistname = query.value(1).toString();
        if(!hasmusic)
        {
            qsql = "INSERT INTO music VALUES(NULL,'" + ui->musicNameEdit->text() +"','" + ui->singerEdit->text() +"','" + ui->albumEdit->text() +"','" + ui->showPath->text() +"', 1)";
            query.exec(qsql);
            int tmpmid = query.lastInsertId().toInt(), tmplid = p->curListId;
            qsql = "INSERT INTO music_list VALUES(NULL," + QString::number(tmplid) + "," + QString::number(tmpmid) + ")";
            query.exec(qsql);
            p->db.close();
            p->listReset(curlistname);
            this->close();
        }
        else
        {
            QString tmpid = query.value(0).toString();
            qsql = "SELECT * FROM music_list WHERE lid = " + QString::number(p->curListId) + " AND mid = " + tmpid;
            query.exec(qsql);
            if(!query.first())
            {
                qsql = "INSERT INTO music_list VALUES(NULL," + QString::number(p->curListId) + "," + tmpid + ")";
                query.exec(qsql);
                p->db.close();
                p->listReset(curlistname);
                this->close();
            }
            else
            {
                QMessageBox *msgBox;
                msgBox = new QMessageBox("错误",
                                         "这首歌在歌单里了哦",
                                         QMessageBox::Critical,
                                         QMessageBox::Ok | QMessageBox::Default,
                                         QMessageBox::Cancel | QMessageBox::Escape,
                                         0);
                msgBox->show();
            }
        }
    }
}

void addmusicDialog::on_cancelBtn_clicked()
{
    this->close();
}

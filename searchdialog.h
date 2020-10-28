#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include<QMap>
#include<QStringList>
#include<QListWidget>
namespace Ui {
class searchDialog;
}

class searchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit searchDialog(QWidget *parent = nullptr);
    ~searchDialog();
    QString searchContent;
    QVector<QStringList> infolist;

private slots:
    void on_pushButton_clicked();

    void on_searchList_itemClicked(QListWidgetItem *item);

private:
    Ui::searchDialog *ui;

};

#endif // SEARCHDIALOG_H

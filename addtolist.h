#ifndef ADDTOLIST_H
#define ADDTOLIST_H

#include <QDialog>
#include<QListWidget>
namespace Ui {
class addToList;
}

class addToList : public QDialog
{
    Q_OBJECT

public:
    explicit addToList(QWidget *parent = nullptr);
    ~addToList();

private slots:
    void on_closeBtn_clicked();

    void on_allLists_itemClicked(QListWidgetItem *item);

private:
    Ui::addToList *ui;
    QVector<int>listid;
};

#endif // ADDTOLIST_H

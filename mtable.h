#ifndef MTABLE_H
#define MTABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QColor>

class MTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit MTable(QWidget *parent = nullptr);
protected:
    void leaveEvent(QEvent *event);//鼠标移出事件
public slots:
    void cellEntered(int,int);//鼠标移动事件，返回item的行和列号
private:
    QColor lastRowBkColor;//存储item之前的颜色，这里是透明的，默认设置为透明
    int previousColorRow;// 鼠标移动过的上一行的行号
    void setRowColor(int,QColor);
};


#endif // MTABLE_H

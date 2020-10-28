#include "mtable.h"
#include <QDebug>
#include <QMouseEvent>

MTable::MTable(QWidget *parent) : QTableWidget(parent)
{
    this->setMouseTracking(true);//设置可捕获鼠标移动事件，很重要
    lastRowBkColor = QColor(0x00,0xff,0x00,0x00);//透明颜色
    previousColorRow = -1;
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(this,SIGNAL(cellEntered(int,int)),this,SLOT(cellEntered(int,int)));
}

void MTable::leaveEvent(QEvent *event)
{
    QTableWidgetItem *item = 0;

    //还原上一行的颜色
    item = this->item(previousColorRow, 0);
    if (item != 0)
    {
        this->setRowColor(previousColorRow, lastRowBkColor);
    }
}

void MTable::cellEntered(int row, int column)
{
    QTableWidgetItem *item = 0;

    //还原上一行的颜色
    item = this->item(previousColorRow, 0);
    if (item != 0)
    {
        this->setRowColor(previousColorRow, lastRowBkColor);
    }

    //设置当前行的颜色
    item = this->item(row, column);
    if (item != 0 && !item->isSelected())
    {
        this->setRowColor(row, QColor(193,210,240));
    }

    //设置行的索引
    previousColorRow = row;
}

void MTable::setRowColor(int row, QColor color)
{
    for (int col=0; col<this->columnCount(); col++)
    {
        QTableWidgetItem *item = this->item(row, col);
        item->setBackgroundColor(color);
    }
}

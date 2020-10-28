#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QPushButton>
#include <QListWidgetItem>
#include <QListWidget>
#include <QScrollArea>
#include<windows.h>
#include<windowsx.h>
#include<QFile>
#include<QDir>
#include<QFileInfo>
#include<QFileInfoList>
#include<QDebug>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include<QSqlQuery>
#include<QFileDialog>
#include<QUrl>
#include"newdialog.h"
#include <QScrollBar>
#include<QTableWidgetItem>
#include <QMediaObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include<QAction>
#include<QMenu>
#include<QBitmap>
#include"addmusicdialog.h"
#include"editlistdialog.h"
#include"addtolist.h"
#include"searchdialog.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

extern "C"
{
    #include <libavcodec/avcodec.h>

    #include <libavformat/avformat.h>

    #include <libswscale/swscale.h>

    #include <libavdevice/avdevice.h>

    #include <libavformat/version.h>

    #include <libavutil/time.h>

    #include <libavutil/mathematics.h>
}
//表示一行歌词（一个时间点+对应的歌词文本）
class LyricLine
{
public:
    qint64 time;
    QString text;
    LyricLine(qint64 time, QString text):time(time), text(text){}
};

bool operator <(const LyricLine& A, const LyricLine& B);

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QVector<LyricLine> lines;//歌词存储
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSqlDatabase db;//数据库对象
    int getCurListId();//通过歌单名称获取当前被选中的歌单编号
    int curListId;//当前被选中的歌单编号
    void reloadList();//重新加载歌单列表
    bool isDefault;
    void listReset(QString listName);//重新设置当前被选中的歌单
    int signalRow;//右键菜单发出信号的行
    int curMusicCnt;//当前歌单下歌曲数目
    QVector<int> listIdx, musicIdx;//歌单与歌曲的顺序与数据库id的映射
    void changeVolume(int values);//改变音量
    QPixmap PixmapToRound(QPixmap &src, int radius);//将Pixmap转成圆形
    QList<QStringList> playlistInfo;//与当前播放列表对应的信息
    //当前播放器
    QMediaPlayer *player;
    //当前播放列表
    QMediaPlaylist *playlist;

    QString searchContent;
protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void styleInit();
    void dbInit();
    void updateInfo();


private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);//点击歌单名称后重置歌单

    void on_insertBtn_clicked();//插入新歌单

    void on_editBtn_clicked();//编辑歌单

    void on_defaultList_itemClicked(QListWidgetItem *item);//设置为所有歌曲

    void on_addmusicBtn_clicked();//添加歌曲

    void on_playBtn_clicked();//播放整个歌单

    void showMenu(QPoint);//展示右键菜单

    void on_musicTable_cellDoubleClicked(int row, int column);//播放单首歌曲

    void add2List();//右键菜单的添加到歌单

    void delFromList();//右键菜单的从歌单删除

    void on_changeStackBtn_clicked();//切换歌词页与歌单详情页

    void on_openBtn_clicked();//切换歌词页与歌单详情页

    void on_pauseBtn_clicked();//播放与暂停
    void on_nextBtn_clicked();//下一首
    void on_prevBtn_clicked();//上一首
    void updatePosition(qint64 position);//更新位置
    void updateDuration(qint64 duration);//更新状态
    void setPosition(int position);//设置位置
    void updatePlayBtn();//更新播放/暂停键的图标

    void on_modeBtn_clicked();//改变播放模式

    void on_searchBtn_clicked();//显示搜索框

    void on_settingBtn_clicked();//显示信息

private:
    Ui::MainWindow *ui;
    QPoint clickPos;         // 鼠标相对于窗口的位置，不是相对屏幕的位置
    int boundaryWidth;



    //将歌词文件的内容处理为歌词结构的QList
    bool lyric_process(QString filePath);
    //根据时间找到对应位置的歌词
    int lyric_getIndex(qint64 position);
    //显示当前播放进度的歌词
    void lyric_show(qint64 position);
    //根据下标获得歌词内容
    QString lyric_getLyricText(int index);
    //清空歌词Label
    void lyric_clear();

    QImage picImage;//存储MP3元数据的图像

};
#endif // MAINWINDOW_H

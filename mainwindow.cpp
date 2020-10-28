#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//隐藏原生标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);
    //setWindowOpacity(0.9);//设置整体透明度
    boundaryWidth = 4;
    curListId = 1;//设为全部歌曲
    styleInit();
    dbInit();
    isDefault = false;
    //播放器初始化
    player= new QMediaPlayer(this);
    playlist=new QMediaPlaylist;
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player->setPlaylist(playlist);
    player->stop();
    ui->volumeSlider->setValue(100);
    ui->progressBar->setToolTip(("播放进度"));
    ui->stack->setCurrentIndex(0);
    connect(ui->progressBar, &QAbstractSlider::valueChanged, this, &MainWindow::setPosition);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::updatePosition);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::updateDuration);
    connect(ui->volumeSlider,&QSlider::valueChanged,this,&MainWindow::changeVolume);//音量进度条置的改变->音量改变
    connect(player, &QMediaPlayer::currentMediaChanged, this, &MainWindow::updateInfo);//获取元数据
    connect(player, &QMediaPlayer::stateChanged, this, &MainWindow::updatePlayBtn);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* msg = (MSG*)message;
    switch(msg->message)
    {
    case WM_NCHITTEST:
        int xPos = GET_X_LPARAM(msg->lParam) - this->frameGeometry().x();
        int yPos = GET_Y_LPARAM(msg->lParam) - this->frameGeometry().y();
        if(xPos < boundaryWidth && yPos<boundaryWidth)                    //左上角
            *result = HTTOPLEFT;
        else if(xPos>=width()-boundaryWidth&&yPos<boundaryWidth)          //右上角
            *result = HTTOPRIGHT;
        else if(xPos<boundaryWidth&&yPos>=height()-boundaryWidth)         //左下角
            *result = HTBOTTOMLEFT;
        else if(xPos>=width()-boundaryWidth&&yPos>=height()-boundaryWidth)//右下角
            *result = HTBOTTOMRIGHT;
        else if(xPos < boundaryWidth)                                     //左边
            *result =  HTLEFT;
        else if(xPos>=width()-boundaryWidth)                              //右边
            *result = HTRIGHT;
        else if(yPos<boundaryWidth)                                       //上边
            *result = HTTOP;
        else if(yPos>=height()-boundaryWidth)                             //下边
            *result = HTBOTTOM;
        else              //其他部分不做处理，返回false，留给其他事件处理器处理
            return false;
        return true;
    }
    return false;         //此处返回false，留给其他事件处理器处理
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
        clickPos=e->pos();
}
void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons()&Qt::LeftButton)
        move(e->pos()+pos()-clickPos);
}

void MainWindow::styleInit()
{
    QListWidgetItem *ite = new QListWidgetItem(ui->defaultList);
    ite->setText(QString("所有音乐"));
    //    ite->setFlags(ite->flags() & ~Qt::ItemIsSelectable);
    ui->defaultList->setFrameShape(QListWidget::NoFrame);
    ui->listWidget->setFrameShape(QListWidget::NoFrame);
    ui->defaultList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->defaultList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->defaultList->setStyleSheet("QListWidget{color:rgb(173,175,178); border:0px solid gray;}"
                                   "QListWidget::Item{height:30px;border:0px solid gray;padding-left:15;}"
                                   "QListWidget::Item:hover{color:rgb(0,0,0);background:transparent;border:0px solid gray;}"
                                   "QListWidget::Item:selected{border-image:url(:/img/ico/listwidget_h.png); color:rgb(0,0,0);background:#00FFFFFF;background-color:transparent;border-width:0;}"
                                   );

    //歌单列表初始化
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setStyleSheet("QListWidget{color:rgb(173,175,178); border:0px solid gray;}"
                                  "QListWidget::Item{height:30px;border:0px solid gray;padding-left:15;}"
                                  "QListWidget::Item:hover{color:rgb(0,0,0);background:transparent;border:0px solid gray;}"
                                  "QListWidget::Item:selected{border-image:url(:/img/ico/listwidget_h.png); color:rgb(0,0,0);background:#00FFFFFF;background-color:transparent;border-width:0;}"
                                  );
    ui->musicTable->setColumnCount(4);
    ui->musicTable->setRowCount(10);//qt巨呆的设计，不设置默认显示0行
    ui->musicTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->musicTable->setFrameShape(QAbstractScrollArea::NoFrame);
    QStringList headers;
    headers << "序号" << "音乐标题" << "歌手" << "专辑";
    //设置表头自适应
    ui->musicTable->setHorizontalHeaderLabels(headers);
    ui->musicTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->musicTable->resizeColumnsToContents();
    ui->musicTable->setAlternatingRowColors(true);//设置双色交替
    ui->musicTable->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可编辑
    ui->musicTable->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->musicTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->musicTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->musicTable->horizontalScrollBar()->setDisabled(true);
    ui->musicTable->verticalScrollBar()->setDisabled(true);
    ui->musicTable->horizontalHeader()->setVisible(true);
    ui->musicTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->musicTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showMenu(QPoint)));
}

void MainWindow::dbInit()
{
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db=QSqlDatabase::addDatabase("QSQLITE");
        QString dbPath = QCoreApplication::applicationDirPath() + "//local.db";
        db.setDatabaseName(dbPath);
    }
    if(!db.open())
        qDebug() << db.lastError();
    QSqlQuery query(db);
    //判断三张表是否初始化，未初始化则进行初始化
    //歌单表
    query.exec("select * from sqlite_master where type='table' and name='list'");
    if(!query.next())
    {
        QString createSql = QString("CREATE TABLE list (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, intro TEXT, avatar TEXT)");
        query.prepare(createSql);
        if(!query.exec())
        {
            qDebug() << query.lastError();
        }
        query.exec("INSERT INTO list VALUES(null, '我喜欢的', '红心歌单', ':/img/avatar.png')");
    }

    //音乐表
    query.exec("select * from sqlite_master where type='table' and name='music'");
    if(!query.next())
    {
        QString createSql = QString("CREATE TABLE music (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, art TEXT NOT NULL, album TEXT NOT NULL, path TEXT NOT NULL, islocal INTEGER)");
        query.prepare(createSql);
        if(!query.exec())
        {
            qDebug() << query.lastError();
        }
    }

    //外键表
    query.exec("select * from sqlite_master where type='table' and name='music_list'");
    if(!query.next())
    {
        QString createSql = QString("CREATE TABLE music_list (id INTEGER PRIMARY KEY AUTOINCREMENT, lid INT NOT NULL, mid INT NOT NULL)");
        query.prepare(createSql);
        if(!query.exec())
        {
            qDebug() << query.lastError();
        }
    }
    reloadList();
    db.close();
    listReset(QString("我喜欢的"));
    QList<QListWidgetItem *> items = ui->listWidget->findItems("我喜欢的", Qt::MatchExactly);
    items[0]->setSelected(true);
}

void MainWindow::reloadList()
{
    ui->listWidget->clear();
    listIdx.clear();
    db.open();
    QSqlQuery query(db);
    query.exec("select * from list");
    while(query.next())
    {
        QListWidgetItem *ite = new QListWidgetItem(ui->listWidget);
        ite->setText(query.value(1).toString());
        ite->setIcon(QIcon(":/img/ico/playlist_n.png"));
        listIdx.push_back(query.value(0).toInt());
        if(query.value(0).toInt() == curListId)
        {
            ui->listName->setText(query.value(1).toString());
            QString listIntro = query.value(2).toString();
            if(listIntro.size() == 0)
            {
                ui->introTip->setText("");
                ui->listDesc->setText("");
            }
            else
            {
                ui->listDesc->setText(listIntro);
                ui->introTip->setText("简介");
            }
        }
    }
}

void MainWindow::listReset(QString listName)
{

    ui->listDesc->clear();
    musicIdx.clear();
    ui->musicTable->clearContents();
    ui->listName->setText(listName);
    db.open();
    QSqlQuery query(db);
    if(listName == "所有歌曲")
    {
        curListId = 0;
        ui->editBtn->hide();
        ui->addmusicBtn->hide();
        ui->listDesc->setText("我的本地音乐库");
        ui->introTip->show();
        ui->listAvatar->setPixmap(QPixmap(":/img/avatar.png").scaled(ui->listAvatar->width(), ui->listAvatar->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        query.exec("SELECT * FROM music");
        int tempi = 0;
        while(query.next())
        {
            ui->musicTable->setItem(tempi, 0, new QTableWidgetItem(QString::number(tempi + 1)));
            ui->musicTable->setItem(tempi, 1, new QTableWidgetItem(query.value(1).toString()));
            ui->musicTable->setItem(tempi, 2, new QTableWidgetItem(query.value(2).toString()));
            ui->musicTable->setItem(tempi, 3, new QTableWidgetItem(query.value(3).toString()));
            musicIdx.push_back(query.value(0).toInt());
            tempi++;
        }
        ui->countLabel->setText("歌曲数：\n" + QString::number(tempi));
        curMusicCnt = tempi;
        int tmpH = max(tempi*100, 350);
        ui->musicTable->setMinimumSize(QSize(0, tmpH));
    }
    else
    {
        query.exec("SELECT * FROM list WHERE name = '" + listName + "'");
        query.first();
        auto listId = query.value(0).toString();
        curListId = listId.toInt();
        auto listIntro = query.value(2).toString();
        if(listIntro.size() == 0) ui->introTip->hide();
        else
        {
            ui->listDesc->setText(listIntro);
            ui->introTip->show();
        }
        ui->addmusicBtn->show();
        if(listName == "我喜欢的") ui->editBtn->hide();
        else ui->editBtn->show();
        ui->listAvatar->setPixmap(QPixmap(query.value(3).toString()).scaled(ui->listAvatar->width(), ui->listAvatar->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        query.exec("SELECT * FROM music_list WHERE lid = " + listId);
        QVector<int> v;
        while(query.next())
        {
            v.push_back(query.value(2).toInt());
        }
        for(int i = 0; i < v.size(); i++)
        {
            if(!query.exec("SELECT * FROM music WHERE id = " + QString::number(v[i]))) qDebug() << query.lastError();
            query.first();
            ui->musicTable->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
            ui->musicTable->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
            ui->musicTable->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
            ui->musicTable->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
            musicIdx.push_back(query.value(0).toInt());
        }
        curMusicCnt = v.size();
        int tmpH = max(v.size()*100, 350);
        ui->musicTable->setMinimumSize(QSize(0, tmpH));
        ui->countLabel->setText("歌曲数：\n" + QString::number(v.size()));
    }

    db.close();
    for(int i=0;i<ui->musicTable->rowCount();i++){
        for(int j=0;j<ui->musicTable->columnCount();j++){
            if(ui->musicTable->item(i,j)!=NULL){               //一定要先判断非空，否则会报错
                ui->musicTable->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            }
        }
    }
}
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    ui->defaultList->clearSelection();
    listReset(item->text());
}

void MainWindow::on_insertBtn_clicked()
{
    newDialog *ndialog = new newDialog(this);
    ndialog->setModal(true);
    ndialog->show();
    ndialog->move(ui->insertBtn->geometry().x() + 20,ui->insertBtn->geometry().y() + ui->insertBtn->geometry().width());
}

void MainWindow::on_editBtn_clicked()
{
    editlistDialog *edialog = new editlistDialog(this);
    edialog->setModal(true);
    edialog->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    edialog->show();
}

int MainWindow::getCurListId()
{
    db.open();
    QSqlQuery query(db);
    query.exec("SELECT * FROM list WHERE name = '" + ui->listName->text() + "'");
    query.first();
    int res = query.value(0).toInt();
    db.close();
    return res;
}



void MainWindow::on_defaultList_itemClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    ui->listWidget->clearSelection();
    listReset("所有歌曲");
}

void MainWindow::on_addmusicBtn_clicked()
{
    qDebug() << "called";
    addmusicDialog *amddialog = new addmusicDialog(this);
    amddialog->setModal(true);
    amddialog->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    amddialog->show();
}

void MainWindow::on_playBtn_clicked()
{
    playlist->clear();
    playlistInfo.clear();
    db.open();
    QSqlQuery query(db);
    query.exec("SELECT * FROM music_list where lid = " + QString::number(curListId));
    QVector<int> v;
    while(query.next())
    {
        v.push_back(query.value(2).toInt());
    }
    for(int i = 0; i < v.size(); i++)
    {
        if(!query.exec("SELECT * FROM music WHERE id = " + QString::number(v[i]))) qDebug() << query.lastError();
        query.first();
        playlistInfo.push_back({query.value(0).toString(), query.value(1).toString(), query.value(2).toString(),query.value(3).toString(),query.value(4).toString(),query.value(5).toString()});
        playlist->addMedia(QUrl(query.value(4).toString()));
    }
    player->play();
}


void MainWindow::on_musicTable_cellDoubleClicked(int row, int column)
{
    if(row > curMusicCnt || curMusicCnt == 0) return;
    player->stop();
    playlist->clear();
    playlistInfo.clear();
    db.open();
    QSqlQuery query(db);
    if(!query.exec("SELECT * FROM music WHERE id = " + QString::number(musicIdx[row]))) qDebug() << query.lastError();
    query.first();
    playlistInfo.push_back({query.value(0).toString(), query.value(1).toString(), query.value(2).toString(),query.value(3).toString(),query.value(4).toString(),query.value(5).toString()});
    if(!playlist->addMedia(QUrl(query.value(4).toString()))) qDebug() << "no";
    ui->pauseBtn->setStyleSheet("QPushButton#pauseBtn{border-image: url(:/img/ico/pause_n.png);}"
                                "QPushButton::hover#pauseBtn{border-image: url(:/img/ico/pause_h.png);}"
                                "QPushButton::pressed#pauseBtn{border-image: url(:/img/ico/pause_n.png);}"
                                );
    player->play();
}

void MainWindow::showMenu(const QPoint pos)
{
    //获得鼠标点击的x，y坐标点
    int x = pos.x ();
    int y = pos.y ();
    QModelIndex index = ui->musicTable->indexAt (QPoint(x,y));
    signalRow  = index.row ();//获得QmusicTable列表点击的行数
    if(signalRow > curMusicCnt || curMusicCnt == 0) return;
    //设置菜单选项
    QMenu *menu = new QMenu(ui->musicTable);
    QAction *pnew = new QAction("从歌单中删除",ui->musicTable);
    QAction *pnew1 = new QAction("添加到歌单",ui->musicTable);
    connect (pnew,SIGNAL(triggered()),this,SLOT(delFromList()));
    connect (pnew1,SIGNAL(triggered()),this,SLOT(add2List()));
    menu->addAction(pnew);
    menu->addAction(pnew1);
    menu->move (cursor ().pos ());
    menu->show ();

}

void MainWindow::delFromList()
{
    db.open();
    QSqlQuery query(db);
    if(curListId != 0)
    {
        if(!query.exec("DELETE FROM music_list WHERE mid = " + QString::number(musicIdx[signalRow]) + " AND lid = " + QString::number(curListId))) qDebug() << query.lastError();
        query.exec("SELECT * FROM list WHERE id = " + QString::number(curListId));
        query.first();
        listReset(query.value(1).toString());
        db.close();
    }
    else
    {
        if(!query.exec("DELETE FROM music WHERE id = " + QString::number(musicIdx[signalRow]))) qDebug() << query.lastError();
        if(!query.exec("DELETE FROM music_list WHERE mid = " + QString::number(musicIdx[signalRow]))) qDebug() << query.lastError();
        listReset("所有歌曲");
        db.close();
    }

}

void MainWindow::add2List()
{
    addToList *aldialog = new addToList(this);
    aldialog->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    aldialog->show();
}


void MainWindow::on_changeStackBtn_clicked()
{
    ui->stack->setCurrentIndex(0);
}

void MainWindow::on_openBtn_clicked()
{
    ui->stack->setCurrentIndex(1);
}


void MainWindow::on_pauseBtn_clicked()
{
    if(player->state()==QMediaPlayer::PlayingState)
    {
        player->pause();

    }
    else if(player->state()==QMediaPlayer::PausedState){

        player->play();
    }
    else if(!playlist->isEmpty() && (player->state()==QMediaPlayer::StoppedState))
    {
        playlist->setCurrentIndex(0);
        player->play();
    }
}

void MainWindow::on_nextBtn_clicked()
{
    playlist->next();
}

void MainWindow::on_prevBtn_clicked()
{
    playlist->previous();
}

void MainWindow::changeVolume(int values)//改变音量
{
    player->setVolume(values);
}

QString formatTime(qint64 timeMilliSeconds)
{
    qint64 seconds = timeMilliSeconds / 1000;
    const qint64 minutes = seconds / 60;
    seconds -= minutes * 60;
    return QStringLiteral("%1:%2")
            .arg(minutes, 2, 10, QLatin1Char('0'))
            .arg(seconds, 2, 10, QLatin1Char('0'));
}
void MainWindow::updatePosition(qint64 position)
{
    ui->progressBar->setValue(static_cast<int>(position));
    ui->curProgressLabel->setText(formatTime(position));
    ui->durationLabel->setText(formatTime(player->duration()));
    if(playlist->currentIndex()>=0) lyric_show(position);
}

void MainWindow::updateDuration(qint64 duration)
{
    ui->progressBar->setRange(0, static_cast<int>(duration));
    ui->progressBar->setEnabled(static_cast<int>(duration) > 0);
    ui->progressBar->setPageStep(static_cast<int>(duration) / 10);
    if(!(static_cast<int>(duration) > 0))
    {
        lyric_clear();
    }
}

void MainWindow::setPosition(int position)
{
    // avoid seeking when the slider value change is triggered from updatePosition()
    if (qAbs(player->position() - position) > 99)
        player->setPosition(position);
}

void MainWindow::updatePlayBtn()
{
    if(player->state()==QMediaPlayer::PlayingState)
    {

        ui->pauseBtn->setStyleSheet("QPushButton#pauseBtn{border-image: url(:/img/ico/pause_n.png);}"
                                    "QPushButton::hover#pauseBtn{border-image: url(:/img/ico/pause_h.png);}"
                                    "QPushButton::pressed#pauseBtn{border-image: url(:/img/ico/pause_n.png);}"
                                    );
    }
    else
    {
        ui->pauseBtn->setStyleSheet("QPushButton#pauseBtn{border-image: url(:/img/ico/play_n.png);}"
                                    "QPushButton::hover#pauseBtn{border-image: url(:/img/ico/play_h.png);}"
                                    "QPushButton::pressed#pauseBtn{border-image: url(:/img/ico/play_n.png);}"
                                    );
    }
}

void MainWindow::updateInfo()
{
    if(playlist->currentIndex() == -1) return;
    picImage=QImage(":/img/avatar.png");
    QString tmpfile = playlistInfo[playlist->currentIndex()][4];
    if(tmpfile.right(3) == "mp3")
    {
        AVFormatContext *fmt_ctx = NULL;
        if (avformat_open_input(&fmt_ctx, tmpfile.toLocal8Bit().data(), NULL, NULL))
        {
            qDebug() << "Fail to open file";
        }
        if (fmt_ctx->iformat->read_header(fmt_ctx) >= 0)
        {
            for (int i = 0; i < fmt_ctx->nb_streams; i++)
            {
                if (fmt_ctx->streams[i]->disposition & AV_DISPOSITION_ATTACHED_PIC) {
                    AVPacket pkt = fmt_ctx->streams[i]->attached_pic;
                    //使用QImage读取完整图片数据（注意，图片数据是为解析的文件数据，需要用QImage::fromdata来解析读取）
                    picImage = QImage::fromData((uchar*)pkt.data, pkt.size);
                    break;
                }
            }
        }
    }
    ui->openBtn->setIcon(QPixmap::fromImage(picImage.scaled(ui->openBtn->width(),ui->openBtn->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    ui->openBtn->setIconSize(picImage.rect().size());

    lyric_process(playlistInfo[playlist->currentIndex()][4].replace(".mp3",".lrc"));
    ui->curMusicNameLabel->setText(playlistInfo[playlist->currentIndex()][1]);
    ui->curArtistNameLabel->setText(playlistInfo[playlist->currentIndex()][2]);
    ui->musicNameLabel->setText(playlistInfo[playlist->currentIndex()][1]);
    ui->artistNameLabel->setText(playlistInfo[playlist->currentIndex()][2]);
    ui->albumNameLabel->setText(playlistInfo[playlist->currentIndex()][3]);
    QPixmap tmpLay = QPixmap::fromImage(picImage).scaled(150,150, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->underLabel->setPixmap(PixmapToRound(tmpLay, 150));
}

//重载比较（歌词按时间排序）
bool operator <(const LyricLine& A, const LyricLine& B){
    return A.time<B.time;
}

bool MainWindow::lyric_process(QString filePath)
{
    QFile lyricFile(filePath);
    lyricFile.open(QFile::ReadOnly);
    QString content(QString::fromLocal8Bit(lyricFile.readAll()));
    lyricFile.close();

    //先清空歌词
    lines.clear();

    const QRegExp rx("\\[(\\d+):(\\d+(\\.\\d+)?)\\]"); //用来查找时间标签的正则表达式

    // 步骤1
    int pos = rx.indexIn(content);
    if (pos == -1) {
        return false;
    }
    else {
        int lastPos;
        QList<int> timeLabels;
        do {
            // 步骤2
            timeLabels << (rx.cap(1).toInt() * 60 + rx.cap(2).toDouble()) * 1000;
            lastPos = pos + rx.matchedLength();
            pos = rx.indexIn(content, lastPos);
            if (pos == -1) {
                QString text = content.mid(lastPos).trimmed();
                foreach (const int& time, timeLabels)
                    lines.push_back(LyricLine(time, text));
                break;
            }
            // 步骤3
            QString text = content.mid(lastPos, pos - lastPos);
            if (!text.isEmpty()) {
                foreach (const int& time, timeLabels)
                    lines.push_back(LyricLine(time, text.trimmed()));
                timeLabels.clear();
            }
        }
        while (true);
        // 步骤4
        std::stable_sort(lines.begin(), lines.end());
    }
    if (lines.size()) {
        return true;
    }
    return false;
}

int MainWindow::lyric_getIndex(qint64 position)
{
    if(!lines.size()){
        return -1;
    }else{
        if(lines[0].time>=position){
            return 0;
        }
    }
    int i=1;
    for(i=1;i<lines.size();i++){
        if(lines[i-1].time<position && lines[i].time>=position){
            return i-1;
        }
    }
    return lines.size()-1;
}

void MainWindow::lyric_show(qint64 position)
{
    int index=lyric_getIndex(position);
    if(index==-1){
        ui->label_3i->setText("");
        ui->label_2i->setText("");
        ui->label_1i->setText("");
        ui->label_i->setText(u8"当前歌曲无歌词");
        ui->label_i1->setText("");
        ui->label_i2->setText("");
        ui->label_i3->setText("");
    }else{
        ui->label_3i->setText(lyric_getLyricText(index-3));
        ui->label_2i->setText(lyric_getLyricText(index-2));
        ui->label_1i->setText(lyric_getLyricText(index-1));
        ui->label_i->setText(lyric_getLyricText(index));
        ui->label_i1->setText(lyric_getLyricText(index+1));
        ui->label_i2->setText(lyric_getLyricText(index+2));
        ui->label_i3->setText(lyric_getLyricText(index+3));
    }
}

QString MainWindow::lyric_getLyricText(int index)
{
    if(index<0 || index>=lines.size()){
        return "";
    }else{
        return lines[index].text;
    }
}

void MainWindow::lyric_clear()
{
    ui->label_3i->setText("");
    ui->label_2i->setText("");
    ui->label_1i->setText("");
    ui->label_i->setText("");
    ui->label_i1->setText("");
    ui->label_i2->setText("");
    ui->label_i3->setText("");
}

QPixmap MainWindow::PixmapToRound(QPixmap &src, int radius)
{
    if (src.isNull()) {
        return QPixmap();
    }
    QSize size(2*radius, 2*radius);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), 99, 99);
    QPixmap image = src.scaled(size);
    image.setMask(mask);
    return image;
}

void MainWindow::on_modeBtn_clicked()
{
    if(playlist->playbackMode()==QMediaPlaylist::Loop){
        ui->modeBtn->setStyleSheet("QPushButton#modeBtn{border-image: url(:/img/ico/random_n.png);}"
                                   "QPushButton::hover#modeBtn{border-image: url(:/img/ico/random_h.png);}"
                                   "QPushButton::pressed#modeBtn{border-image: url(:/img/ico/random_n.png);}"
                                   );
        playlist->setPlaybackMode(QMediaPlaylist::Random);
    }
    else if(playlist->playbackMode()==QMediaPlaylist::Random){
        ui->modeBtn->setStyleSheet("QPushButton#modeBtn{border-image: url(:/img/ico/loop1_n.png);}"
                                   "QPushButton::hover#modeBtn{border-image: url(:/img/ico/loop1_h.png);}"
                                   "QPushButton::pressed#modeBtn{border-image: url(:/img/ico/loop1_n.png);}"
                                   );
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    }
    else if(playlist->playbackMode()==QMediaPlaylist::CurrentItemInLoop){
        ui->modeBtn->setStyleSheet("QPushButton#modeBtn{border-image: url(:/img/ico/loop_n.png);}"
                                   "QPushButton::hover#modeBtn{border-image: url(:/img/ico/loop_h.png);}"
                                   "QPushButton::pressed#modeBtn{border-image: url(:/img/ico/loop_n.png);}"
                                   );
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
    }
}

void MainWindow::on_searchBtn_clicked()
{
    searchContent = ui->searchBar->text();
    searchDialog *ssdialog = new searchDialog(this);
    ssdialog->setModal(true);
    ssdialog->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    ssdialog->show();
    ssdialog->setGeometry(ui->searchBar->geometry().x() + MainWindow::x(),ui->searchBar->geometry().y() + MainWindow::y() + ui->searchBar->height(), 400, 400);
}

void MainWindow::on_settingBtn_clicked()
{
    QMessageBox box;//也可以创建其它对话框，如Qdialog，可以设置窗口大小，文本显示位置等等
    box.setText("介绍与使用教程见：https://www.matrix72.top/dev/qt-player");
    box.setTextInteractionFlags(Qt::TextSelectableByMouse);//设置文本可选中
    box.exec();//函数用于弹出对话框，会阻塞，关闭对话框后解除阻塞
}

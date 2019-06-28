﻿#include "reviewwidget.h"
#include "ui_reviewwidget.h"

#include "videowidget.h"
#include "src/tool.h"
#include <QMediaService>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <QVideoProbe>
#include <QVideoWidget>
#include <QAudioProbe>
#include <QMediaMetaData>
#include <QtWidgets>
#include <player/videowidget.h>
#include "player/qtavplayercontrols.h"
#include "QtAVWidgets/QtAVWidgets.h"

ReviewWidget::ReviewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReviewWidget)
{
    ui->setupUi(this);
    //创建媒体播放器
    m_player = new QtAV::AVPlayer(this);
    //连接信号和槽
    connect(m_player, &QtAV::AVPlayer::durationChanged, this, &ReviewWidget::durationChanged);//最终位置更改
    connect(m_player, &QtAV::AVPlayer::positionChanged, this, &ReviewWidget::positionChanged);//位置更改
    connect(m_player, &QtAV::AVPlayer::stateChanged, this, &ReviewWidget::stateChanged);//状态更改
    //添加默认的输出窗口
    m_videoWidget = new QtAVVideoWidget(this);
    m_player->addVideoRenderer(m_videoWidget->video_render());
    //将显示窗口添加到ui布局
    ui->verticalLayout_2->addWidget(m_videoWidget);
    //初始化进度条范围
    ui->m_slider->setRange(0, m_player->duration() / 1000);
    connect(ui->m_slider, &QSlider::sliderMoved, this, &ReviewWidget::seek);
    connect(ui->m_slider,&QSlider::valueChanged,this,&ReviewWidget::SendCoordinatesToBus);//更新函数坐标位置
    connect(ui->openButton, &QPushButton::clicked, this, &ReviewWidget::open);
    ui->controls->setState(m_player->state());
    ui->controls->setVolume(m_player->audio()->volume());
    connect(ui->controls, &QtAVPlayerControls::play,this, &ReviewWidget::play);//播放
    connect(ui->controls, &QtAVPlayerControls::pause, this, &ReviewWidget::pause);//暂停
    connect(ui->controls, &QtAVPlayerControls::stop, this, &ReviewWidget::stop);//停止
    connect(ui->controls, &QtAVPlayerControls::previous, this, &ReviewWidget::previousClicked);//前一个
    connect(ui->controls, &QtAVPlayerControls::changeRate, m_player, &QtAV::AVPlayer::setSpeed);//设置播放速度
    connect(ui->controls, &QtAVPlayerControls::stop, m_videoWidget, QOverload<>::of(&QVideoWidget::update));//设置更新
    connect(m_player, SIGNAL(stateChanged(QtAV::AVPlayer::State)), ui->controls, SLOT(setState(QtAV::AVPlayer::State)));//转变函数
    metaDataChanged();
}

ReviewWidget::~ReviewWidget()
{
    delete ui;
}
bool ReviewWidget::isPlayerAvailable() const
{
    //return m_player->isAvailable();
    return m_player->isLoaded();
}

void ReviewWidget::open()
{
//    QFileDialog fileDialog(this);
//    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
//    fileDialog.setWindowTitle(tr("Open Files"));
//    QStringList supportedMimeTypes = m_player->supportedMimeTypes();
//    if (!supportedMimeTypes.isEmpty()) {
//        supportedMimeTypes.append("audio/x-m3u"); // MP3 playlists
//        fileDialog.setMimeTypeFilters(supportedMimeTypes);
//    }
//    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
//    if (fileDialog.exec() == QDialog::Accepted){
//        //将文件添加到播放列表
//        addToPlaylist(fileDialog.selectedUrls());
//    }
    sql_choose_dialog_=new RecordSelectDialog(this);
    connect(sql_choose_dialog_,&RecordSelectDialog::SendShowMessage,this,&ReviewWidget::GetMainShowMessage);
    sql_choose_dialog_->show();
}

static bool isPlaylist(const QUrl &url) // Check for ".m3u" playlists.
{
    if (!url.isLocalFile())
        return false;
    const QFileInfo fileInfo(url.toLocalFile());
    return fileInfo.exists() && !fileInfo.suffix().compare(QLatin1String("m3u"), Qt::CaseInsensitive);
}

void ReviewWidget::addToPlaylist(const QList<QString> &urls)
{
//    for (auto &url: urls) {
//        if (isPlaylist(url)){
//            //m_playlist->load(url);

//        }else{
//            //m_playlist->addMedia(url);
//        }

//    }
    if(!urls.isEmpty()){
        m_player->setFile(urls.at(0));
    }
}

//void ReviewWidget::setCustomAudioRole(const QString &role)
//{
//    m_player->setCustomAudioRole(role);
//}
//持续时间更改
void ReviewWidget::durationChanged(qint64 duration)
{
    //m_duration = duration / 1000;
    m_duration = duration/1000;//更改系数，使得更加完整
    ui->m_slider->setMaximum(m_duration);
}
//位置更改
void ReviewWidget::positionChanged(qint64 progress)
{
    if (!ui->m_slider->isSliderDown())
        ui->m_slider->setValue(progress / 1000);
    //更新数字信息
    updateDurationInfo(progress / 1000);
    //更新位置发射
    //SendCoordinatesToBus(progress/1000);
}
//元数据更改
void ReviewWidget::metaDataChanged()
{
//    if (m_player->isMetaDataAvailable()) {
//        setTrackInfo(QString("%1 - %2")
//                .arg(m_player->metaData(QMediaMetaData::AlbumArtist).toString())
//                .arg(m_player->metaData(QMediaMetaData::Title).toString()));

//        if (m_coverLabel) {
//            QUrl url = m_player->metaData(QMediaMetaData::CoverArtUrlLarge).value<QUrl>();

//            m_coverLabel->setPixmap(!url.isEmpty()
//                    ? QPixmap(url.toString())
//                    : QPixmap());
//        }
//    }
}

void ReviewWidget::previousClicked()
{
    // Go to previous track if we are within the first 5 seconds of playback
    // Otherwise, seek to the beginning.
    if (m_player->position() <= 5000)
    {
        //m_playlist->previous();
    }else{
        m_player->setPosition(0);
    }
}

void ReviewWidget::jump(const QModelIndex &index)
{
    if (index.isValid()) {
        //m_playlist->setCurrentIndex(index.row());
        m_player->play();
    }
}

void ReviewWidget::playlistPositionChanged(int currentItem)
{
    //m_playlistView->setCurrentIndex(m_playlistModel->index(currentItem, 0));
}

void ReviewWidget::seek(int seconds)
{
    m_player->setPosition(seconds * 1000);
}

void ReviewWidget::statusChanged(QtAV::MediaStatus status)
{
    handleCursor(status);

    // handle status message
    switch (status) {
    case QtAV::MediaStatus::UnknownMediaStatus:
    case QtAV::MediaStatus::NoMedia:
    case QtAV::MediaStatus::LoadedMedia:
        setStatusInfo(QString());
        break;
    case QtAV::MediaStatus::LoadingMedia:
        setStatusInfo(tr("Loading..."));
        break;
    case QtAV::MediaStatus::BufferingMedia:
    case QtAV::MediaStatus::BufferedMedia:
        setStatusInfo(tr("Buffering %1%").arg(m_player->bufferMode()));
        break;
    case QtAV::MediaStatus::StalledMedia:
        setStatusInfo(tr("Stalled %1%").arg(m_player->bufferMode()));
        break;
    case QtAV::MediaStatus::EndOfMedia:
        QApplication::alert(this);
        break;
    case QtAV::MediaStatus::InvalidMedia:
        displayErrorMessage();
        break;
    }
}

void ReviewWidget::stateChanged(QtAV::AVPlayer::State state)
{
    qDebug()<<"------ state changed ------";
}

void ReviewWidget::handleCursor(QtAV::MediaStatus status)
{
#ifndef QT_NO_CURSOR
    if (status == QtAV::MediaStatus::LoadingMedia ||
        status == QtAV::MediaStatus::BufferingMedia ||
        status == QtAV::MediaStatus::StalledMedia)
        setCursor(QCursor(Qt::BusyCursor));
    else
        unsetCursor();
#endif
}

void ReviewWidget::bufferingProgress(int progress)
{
    if (m_player->mediaStatus() == QMediaPlayer::StalledMedia)
        setStatusInfo(tr("Stalled %1%").arg(progress));
    else
        setStatusInfo(tr("Buffering %1%").arg(progress));
}

void ReviewWidget::videoAvailableChanged(bool available)
{

}
//设置相关信息
void ReviewWidget::setTrackInfo(const QString &info)
{
//    m_trackInfo = info;

//    if (m_statusBar) {
//        m_statusBar->showMessage(m_trackInfo);
//        m_statusLabel->setText(m_statusInfo);
//    } else {
//        if (!m_statusInfo.isEmpty())
//            setWindowTitle(QString("%1 | %2").arg(m_trackInfo).arg(m_statusInfo));
//        else
//            setWindowTitle(m_trackInfo);
//    }
}
//
void ReviewWidget::setStatusInfo(const QString &info)
{
    m_statusInfo = info;

//    if (m_statusBar) {
//        m_statusBar->showMessage(m_trackInfo);
//        m_statusLabel->setText(m_statusInfo);
//    } else {
//        if (!m_statusInfo.isEmpty())
//            setWindowTitle(QString("%1 | %2").arg(m_trackInfo).arg(m_statusInfo));
//        else
//            setWindowTitle(m_trackInfo);
//    }
}
//显示错误信息
void ReviewWidget::displayErrorMessage()
{
    //setStatusInfo(m_player->error());
}
//更新时间信息
void ReviewWidget::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || m_duration) {
        QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60,
            currentInfo % 60, (currentInfo * 1000) % 1000);
        QTime totalTime((m_duration / 3600) % 60, (m_duration / 60) % 60,
            m_duration % 60, (m_duration * 1000) % 1000);
        QString format = "mm:ss";
        if (m_duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    //设置更新时间
    ui->m_labelDuration->setText(tStr);

}


void ReviewWidget::InitQGeoCoordinates(int record_id)
{
    Tool::TestNoteTool("InitQGeoCoordinates",0);
    QJsonObject temp_data;
    temp_data.insert("record_id",record_id);
    QString request_url="http://localhost/re_get_gps.php";
    QJsonObject test=Tool::NetWorkGet(request_url,temp_data);
    QJsonArray result_array=test.value("result").toArray();
    //便利初始化坐标数组
    int array_size=result_array.size();
    for(int i=0;i<array_size;++i){
        QJsonObject temp=result_array.at(i).toObject();//取得第i组数据
        double latitude=temp.value("latitude").toString().toDouble();
        double longitude=temp.value("longitude").toString().toDouble();
        bus_coordinates_list_.append(QGeoCoordinate(latitude,longitude));
    }
    qDebug()<<bus_coordinates_list_.at(10);
    Tool::TestNoteTool("InitQGeoCoordinates",1);
}
void ReviewWidget::SendCoordinatesToBus(int index)
{
    int coordinates_size=bus_coordinates_list_.size();
    //注意这里要检查m_slider的值，应为停止播放的时候可能值为0
    //直接写入函数，发射信号
    int position_index=0;
    if(ui->m_slider->maximum()>0){
        position_index=index*(coordinates_size/ui->m_slider->maximum());
    }else{
        qDebug()<<"this video max slider is 0";
        return;
    }
    qDebug()<<"send positon index:"<<position_index;
    //发射信号更新位置
    if(position_index<coordinates_size){
        emit(SendQGeoCoordinate(bus_coordinates_list_.at(position_index)));
    }else{
        qDebug()<<"this over";
    }
}
void ReviewWidget::GetMainShowMessage(MainSendMessage new_message)
{
    //重新初始化记录
    InitQGeoCoordinates(new_message.record_id);
    //添加播放列表
    //m_playlist->clear();

    addToPlaylist(new_message.video_paths);
    qDebug()<<new_message.start_datetime;

}
void ReviewWidget::play()
{

    this->m_player->play();
    m_player->setState(QtAV::AVPlayer::PlayingState);
}
void ReviewWidget::pause()
{
    this->m_player->pause(!m_player->isPaused());
}
void ReviewWidget::stop()
{
    this->m_player->stop();

}

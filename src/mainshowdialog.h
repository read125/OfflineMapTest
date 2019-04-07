﻿#ifndef MAINSHOWDIALOG_H
#define MAINSHOWDIALOG_H

#include <QDialog>
#include <QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QQuickWidget>
#include <QQuickView>
#include <QVBoxLayout>
#include <QSharedPointer>
#include <QComboBox>
#include <iostream>
#include <memory>
//地图类
#include <QtLocation/private/qdeclarativegeomap_p.h>
class VideoShowWidget;
namespace Ui {
class MainShowDialog;
}

class MainShowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainShowDialog(QWidget *parent = nullptr);
    ~MainShowDialog();

private:
    Ui::MainShowDialog *ui;
    //私有成员类
    QTabWidget* stacked_widget_=nullptr;//中间间隔类
    QQuickWidget* map_page_=nullptr;//qml地图
    VideoShowWidget* video_page_=nullptr;//自定义视频展示widget
    QVBoxLayout* map_show_vbox_layout_=nullptr;// 地图水平布局
    QVBoxLayout* video_show_vbox_layout=nullptr;// 视频水平布局
    std::shared_ptr<QDeclarativeGeoMap> show_map_=nullptr;//地图指针方便获取地图
    QWidget* aggregative_show_page_=nullptr;//综合模式新的page;
    QGridLayout *aggregative_gridLayout_=nullptr;//综合模式的布局，方便设置
    QQuickWidget *speed_show_chart_widget_=nullptr;//仪表盘数据指针，/*ToDo 单独构造它的类，方便复用 */
};

#endif // MAINSHOWDIALOG_H

﻿#include "frmmain.h"
#include "ui_frmmain.h"
#include "iconhelper.h"
#include "myhelper.h"
#include "myapp.h"
#include "frmconfig.h"
#include "frmnvr.h"
#include "frmipc.h"
#include "frmpollconfig.h"

frmMain::frmMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmMain)
{
    ui->setupUi(this);
    //初始化风格
    this->InitStyle();
    //初始化表格
    this->InitForm();
    //初始化右侧按钮
    this->InitMenu();
    //初始化视频
    this->InitVideo();
    //初始化TabWidget
    this->InitTabWidget();
    //初始化视频加载
    this->LoadVideo();
    //初始化NVRIPC
    this->LoadNVRIPC();
}

frmMain::~frmMain()
{
    delete ui;
    if(menuStyle!=nullptr){
        delete [] menuStyle;
    }
    if(menu!=nullptr){
        delete [] menu;
    }
    if(video_vbox_layout_!=nullptr){
        delete [] video_vbox_layout_;
    }
    if(video_control_widget_!=nullptr){
        delete [] video_control_widget_;
    }
    if(map_control_widget_!=nullptr){
        delete [] map_control_widget_;
    }
    if(map_vbox_layout_!=nullptr){
        delete [] map_vbox_layout_;
    }
    if(aggregative_gridLayout_!=nullptr){
        delete [] aggregative_gridLayout_;
    }
    if(speed_chart_widget_!=nullptr){
        delete [] speed_chart_widget_;
    }

}

void frmMain::InitStyle()
{
	this->setStyleSheet("QGroupBox#gboxMain{border-width:0px;}");
    this->setProperty("Form", true);
    //设置窗体标题栏隐藏--Qt::WindowStaysOnTopHint |
    this->setWindowFlags(Qt::FramelessWindowHint |
                         Qt::WindowSystemMenuHint |
                         Qt::WindowMinMaxButtonsHint);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf03d), 11);
    IconHelper::Instance()->SetIcon(ui->labStyle, QChar(0xf103), 12);
}

void frmMain::change_style()
{
    QAction *action = (QAction *)sender();
    QString style = action->text();
    qDebug()<<style;
    if (style==QStringLiteral("淡蓝色")) {
        qDebug()<<style;
        myApp::AppStyle = QStringLiteral(":/image/blue.css");
    } else if (style == QStringLiteral("蓝色")) {
        myApp::AppStyle = QStringLiteral(":/image/dev.css");
    } else if (style == QStringLiteral("灰色")) {
        myApp::AppStyle = QStringLiteral(":/image/gray.css");
    } else if (style == QStringLiteral("黑色")) {
        myApp::AppStyle = QStringLiteral(":/image/black.css");
    } else if (style == QStringLiteral("灰黑色")) {
        myApp::AppStyle = QStringLiteral(":/image/brown.css");
    } else if (style == QStringLiteral("白色")) {
        myApp::AppStyle = QStringLiteral(":/image/white.css");
    } else if (style == QStringLiteral("银色")) {
        myApp::AppStyle = QStringLiteral(":/image/silvery.css");
    }

    myHelper::SetStyle(myApp::AppStyle);
    myApp::WriteConfig();
}

void frmMain::InitForm()
{
    ui->labFull->installEventFilter(this);
    ui->labFull->setProperty("labForm", true);

    ui->labStart->installEventFilter(this);
    ui->labStart->setProperty("labForm", true);

    ui->labNVR->installEventFilter(this);
    ui->labNVR->setProperty("labForm", true);

    ui->labIPC->installEventFilter(this);
    ui->labIPC->setProperty("labForm", true);

    ui->labPollConfig->installEventFilter(this);
    ui->labPollConfig->setProperty("labForm", true);

    ui->labVideoPlayBack->installEventFilter(this);
    ui->labVideoPlayBack->setProperty("labForm", true);

    ui->labConfig->installEventFilter(this);
    ui->labConfig->setProperty("labForm", true);

    ui->labExit->installEventFilter(this);
    ui->labExit->setProperty("labForm", true);

    ui->labStyle->installEventFilter(this);
    ui->labStyle->setProperty("labForm", true);

    menuStyle = new QMenu(this);
    menuStyle->addAction(QStringLiteral("淡蓝色"), this, SLOT(change_style()));
    menuStyle->addAction(QStringLiteral("蓝色"), this, SLOT(change_style()));
    menuStyle->addAction(QStringLiteral("灰色"), this, SLOT(change_style()));
    menuStyle->addAction(QStringLiteral("黑色"), this, SLOT(change_style()));
    menuStyle->addAction(QStringLiteral("灰黑色"), this, SLOT(change_style()));
    menuStyle->addAction(QStringLiteral("白色"), this, SLOT(change_style()));
    menuStyle->addAction(QStringLiteral("银色"), this, SLOT(change_style()));
    menuStyle->setStyleSheet("font: 10pt \"微软雅黑\";");

    ui->lab_Title->setText(myApp::AppTitle);
    this->setWindowTitle(myApp::AppTitle);

    ui->treeMain->header()->setVisible(false);
    ui->treeMain->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void frmMain::InitTabWidget(){
      //添加视频控制
     video_vbox_layout_=new QVBoxLayout();
     ui->video_control_page->setLayout(video_vbox_layout_);
     video_vbox_layout_->setMargin(2);
     video_vbox_layout_->addWidget(ui->widget_main);
     video_control_widget_=ui->widget_main;
     //添加地图控制
     map_control_widget_=new QQuickWidget();
     map_control_widget_->setResizeMode(QQuickWidget::SizeRootObjectToView);
     map_control_widget_->setSource(QUrl("qrc:/qml/MapControlModel.qml"));
     //创建地图布局控件
     map_vbox_layout_=new QVBoxLayout(ui->map_control_page);
     map_vbox_layout_->setMargin(5);//设置边距
     map_vbox_layout_->addWidget(map_control_widget_);

     //获取控制地图
     control_map_=std::shared_ptr<QDeclarativeGeoMap>(map_control_widget_->rootObject()->findChild<QDeclarativeGeoMap *>("control_show_map"));
     //初始化综合模式
     //由于综合模式的特殊性，不得不使用QTabWidget的信号来实现切换
     //QVBoxLayout* aggregative_vbox_layout_=new QVBoxLayout(ui->aggregative_model);
     //初始化网格布局
     aggregative_gridLayout_=new QGridLayout(ui->aggregative_model);
     //设置仪表盘面板
     speed_chart_widget_=new QQuickWidget();
     speed_chart_widget_->setResizeMode(QQuickWidget::SizeRootObjectToView);
     speed_chart_widget_->setSource(QUrl("qrc:/qml/MyCharts.qml"));
}

void frmMain::InitVideo()
{
    tempLab = 0;
    video_max_ = false;
    video_count_=12;
    video_type_="1_16";
    //添加QLab
    for(int i=0;i<video_count_;++i){
        QLabel *widget = new QLabel();
        widget->setObjectName(QString("video%1").arg(i + 1));
        widget->installEventFilter(this);
        widget->setFocusPolicy(Qt::StrongFocus);
        widget->setAlignment(Qt::AlignCenter);
        //二选一可以选择显示文字,也可以选择显示背景图片
        widget->setLineWidth(2);
        widget->setText(QString(QStringLiteral("通道 %1")).arg(i + 1));
        //widget->setPixmap(QPixmap(":/bg_novideo.png"));
        video_labs_.append(widget);
    }
}

void frmMain::InitMenu()
{
    //设置右键
    menu = new QMenu(this);
    menu->setStyleSheet(QStringLiteral("font: 10pt \"微软雅黑\";"));
    menu->addAction(QStringLiteral("删除当前视频"), this, SLOT(delete_video_one()));
    menu->addAction(QStringLiteral("删除所有视频"), this, SLOT(delete_video_all()));
    menu->addSeparator();
    menu->addAction(QStringLiteral("截图当前视频"), this, SLOT(snapshot_video_one()));
    menu->addAction(QStringLiteral("截图所有视频"), this, SLOT(snapshot_video_all()));
    menu->addSeparator();
    //设置切换1画面
    QMenu *menu1 = menu->addMenu(QStringLiteral("切换到1画面"));
    menu1->addAction(QStringLiteral("通道1"), this, SLOT(show_video_1()));
    menu1->addAction(QStringLiteral("通道2"), this, SLOT(show_video_1()));
    menu1->addAction(QStringLiteral("通道3"), this, SLOT(show_video_1()));
    menu1->addAction(QStringLiteral("通道4"), this, SLOT(show_video_1()));
    menu1->addAction(QStringLiteral("通道5"), this, SLOT(show_video_1()));
    menu1->addAction(QStringLiteral("通道6"), this, SLOT(show_video_1()));
    menu1->addAction(QStringLiteral("通道7"), this, SLOT(show_video_1()));
    menu1->addAction(QStringLiteral("通道8"), this, SLOT(show_video_1()));
    menu1->addAction(QStringLiteral("通道9"), this, SLOT(show_video_1()));
    menu1->addAction(QStringLiteral("通道10"), this, SLOT(show_video_1()));
    menu1->addAction(QStringLiteral("通道11"), this, SLOT(show_video_1()));
    menu1->addAction(QStringLiteral("通道12"), this, SLOT(show_video_1()));
//    menu1->addAction(QStringLiteral("通道13"), this, SLOT(show_video_1()));
//    menu1->addAction(QStringLiteral("通道14"), this, SLOT(show_video_1()));
//    menu1->addAction(QStringLiteral("通道15"), this, SLOT(show_video_1()));
//    menu1->addAction(QStringLiteral("通道16"), this, SLOT(show_video_1()));
    //设置切换4画面
    QMenu *menu4 = menu->addMenu(QStringLiteral("切换到4画面"));
    menu4->addAction(QStringLiteral("通道1-通道4"), this, SLOT(show_video_4()));
    menu4->addAction(QStringLiteral("通道5-通道8"), this, SLOT(show_video_4()));
    menu4->addAction(QStringLiteral("通道9-通道12"), this, SLOT(show_video_4()));
   // menu4->addAction(QStringLiteral("通道13-通道16"), this, SLOT(show_video_4()));
    //设置切换6画面
    QMenu *menu6=menu->addMenu(QStringLiteral("切换到6画面"));
    menu6->addAction(QStringLiteral("通道1-通道6"), this, SLOT(show_video_6()));
    menu6->addAction(QStringLiteral("通道7-通道12"), this, SLOT(show_video_6()));
    //设置切换到7画面
    QMenu *menu7 = menu->addMenu(QStringLiteral("切换到7画面"));
    menu7->addAction(QStringLiteral("通道1-通道7"), this, SLOT(show_video_7()));
    menu7->addAction(QStringLiteral("通道6-通道12"), this, SLOT(show_video_7()));
    //设置切换到12画面
    menu->addAction(QStringLiteral("切换到12画面"), this, SLOT(show_video_12()));
}
void frmMain::LoadVideo()
{
    //自动应用最后一次的布局配置
    ChangeVideoLayout();
}

void frmMain::ChangeRtspAddr(int ch, QString rtspAddr)
{
    QStringList rtspAddrs = myApp::RtspAddr12.split("|");
    rtspAddrs[ch] = rtspAddr;

    QString tempRtspAddr12;
    for (int i = 0; i < 12; ++i) {
        tempRtspAddr12 += rtspAddrs[i] + "|";
    }
    myApp::RtspAddr12 = tempRtspAddr12.mid(0, tempRtspAddr12.length() - 1);
}
//加载vcr表格
void frmMain::LoadNVRIPC()
{
    ui->treeMain->clear();

    QSqlQuery queryNVR;
    QString sqlNVR = "select [NVRID],[NVRName],[NVRIP] from [NVRInfo] where [NVRUse]='启用'";
    queryNVR.exec(sqlNVR);

    while (queryNVR.next()) {
        QString tempNVRID = queryNVR.value(0).toString();
        QString tempNVRName = queryNVR.value(1).toString();
        QString tempNVRIP = queryNVR.value(2).toString();

        QTreeWidgetItem *itemNVR = new QTreeWidgetItem
                (ui->treeMain, QStringList(tempNVRName + "[" + tempNVRIP + "]"));
        itemNVR->setIcon(0, QIcon(":/image/nvr.png"));

        QSqlQuery queryIPC;
        QString sqlIPC = QString("select [IPCID],[IPCName],[IPCRtspAddrMain] from [IPCInfo] where [NVRID]='%1' and [IPCUse]='启用' order by [IPCID] asc").arg(tempNVRID);
        queryIPC.exec(sqlIPC);

        while (queryIPC.next()) {
            QString tempIPCName = queryIPC.value(1).toString();
            QString rtspAddr = queryIPC.value(2).toString();

            QStringList temp = rtspAddr.split("/");
            QString ip = temp[2].split(":")[0];

            QTreeWidgetItem *itemIPC = new QTreeWidgetItem(itemNVR, QStringList(QString(tempIPCName + "[" + ip + "]")));
            itemIPC->setIcon(0, QIcon(":/image/ipc_normal.png"));
            itemNVR->addChild(itemIPC);
        }
    }
    ui->treeMain->expandAll();
}

void frmMain::ChangeVideoLayout()
{
    if (myApp::VideoType == "1_4") {
        removelayout();
        change_video_4(0);
    } else if (myApp::VideoType == "5_8") {
        removelayout();
        change_video_4(4);
    } else if (myApp::VideoType == "9_12") {
        removelayout();
        change_video_4(8);
    }
    /*else if (myApp::VideoType == "13_16") {
        removelayout();
        change_video_4(12);
    }*/
    else if (myApp::VideoType == "1_6") {
        removelayout();
        change_video_6(0);
    } else if (myApp::VideoType == "7_12") {
        removelayout();
        change_video_6(6);
    }else if(myApp::VideoType=="1_7"){
        removelayout();
        change_video_7(0);
    }else if(myApp::VideoType=="6_12"){
        removelayout();
        change_video_7(5);
    } else if (myApp::VideoType == "1_12") {
        removelayout();
        change_video_12();
    }else {
        removelayout();
        change_video_4(0);
    }
}

void frmMain::keyPressEvent(QKeyEvent *event)
{
    //空格键进入全屏,esc键退出全屏
    switch(event->key()) {
    case Qt::Key_F1:
        screen_full();
        break;
    case Qt::Key_Escape:
        screen_normal();
        break;
    default:
        QDialog::keyPressEvent(event);
        break;
    }
}

bool frmMain::eventFilter(QObject *obj, QEvent *event)
{
    //将其转为静态指针
    QMouseEvent *MouseEvent = static_cast<QMouseEvent *>(event);
    //设置鼠标双击事件
    if ((event->type() == QEvent::MouseButtonDblClick) &&
            (MouseEvent->buttons() == Qt::LeftButton)) {
        QLabel *labDouble = qobject_cast<QLabel *>(obj);
        if (!video_max_) {//如果通道不是最大化。实现最大化
            removelayout();//将显示的视屏移除
            video_max_ = true;//设置最大化为真
            ui->gridLayout->addWidget(labDouble,0,0);//添加窗口到布局界面
            labDouble->setVisible(true);
        } else {//已经全屏则退回原来的模式
            video_max_ = false;
            ChangeVideoLayout();//更改模式为原来的模式
        }

        labDouble->setFocus();//设置获得焦点事件
        return true;
    } else if (event->type() == QEvent::MouseButtonPress) {//获取简单鼠标点击事件
        if (obj == ui->labFull) {//如果是全屏
            screen_full();//全屏
            return true;
        } else if (obj == ui->labStart) {//启动轮询
            if (ui->labStart->text() == QStringLiteral("启动轮询")) {
                ui->labStart->setText(QStringLiteral("停止轮询"));
            } else {
                ui->labStart->setText(QStringLiteral("启动轮询"));
            }
            return true;
        } else if (obj == ui->labNVR) {//如果是NVR,则开启NVR
            frmNVR nvr;
            nvr.exec();
            LoadNVRIPC();
            return true;
        } else if (obj == ui->labIPC) {//如果是IPC，则开启IPC窗口
            frmIPC ipc;
            ipc.exec();
            LoadNVRIPC();
            return true;
        } else if (obj == ui->labPollConfig) {//开启轮询设置界面
            frmPollConfig pollConfig;
            pollConfig.exec();//显示设置按钮
            return true;
        } else if (obj == ui->labVideoPlayBack) {//设置视频互访
            myHelper::ShowMessageBoxError(QStringLiteral("功能暂未开放!"));
            return true;
        } else if (obj == ui->labConfig) {//设置系统设置
            frmConfig config;
            config.exec();
            ui->lab_Title->setText(myApp::AppTitle);
            this->setWindowTitle(myApp::AppTitle);
            return true;
        } else if (obj == ui->labExit) {//关闭按钮
            on_btnMenu_Close_clicked();
            return true;
        } else if (obj == ui->labStyle) {//风格按钮
            menuStyle->exec(QPoint(myApp::DeskWidth - 155, 31));
            return true;
        } else if (MouseEvent->buttons() == Qt::RightButton) {//途观是鼠标右键的话
            tempLab = qobject_cast<QLabel *>(obj);
            menu->exec(QCursor::pos());//鼠标右键位置
            return true;
        } else {
            tempLab = qobject_cast<QLabel *>(obj);
            //设置当前选中
            ui->lab_Title->setText(QStringLiteral("%1  当前选中[%2]").arg(myApp::AppTitle).arg(tempLab->text()));
            return true;
        }
    }

    return QObject::eventFilter(obj, event);//返回监听事件
}

void frmMain::on_btnMenu_Close_clicked()
{
    exit(0);
}

void frmMain::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void frmMain::delete_video_one()
{

}

void frmMain::delete_video_all()
{

}

void frmMain::snapshot_video_one()
{

}

void frmMain::snapshot_video_all()
{

}

void frmMain::removelayout()
{
    for(int i=0;i<video_count_;++i)
    {
        ui->gridLayout->removeWidget(video_labs_.at(i));
        video_labs_.at(i)->setVisible(false);
    }
}

void frmMain::show_video_1()
{
    myApp::VideoType = "0_0";
    video_max_ = true;
    int index = 0;

    QAction *action = qobject_cast<QAction *>(sender());
    QString name = action->text();
    if (name == QStringLiteral("通道1")) {
        index = 0;
        myApp::VideoType = "0_0";
    } else if (name == QStringLiteral("通道2")) {
        index = 1;
        myApp::VideoType = "0_1";
    } else if (name == QStringLiteral("通道3")) {
        index = 2;
        myApp::VideoType = "0_2";
    } else if (name == QStringLiteral("通道4")) {
        index = 3;
        myApp::VideoType = "0_3";
    } else if (name == QStringLiteral("通道5")) {
        index = 4;
        myApp::VideoType = "0_4";
    } else if (name == QStringLiteral("通道6")) {
        index = 5;
        myApp::VideoType = "0_5";
    } else if (name == QStringLiteral("通道7")) {
        index = 6;
        myApp::VideoType = "0_6";
    } else if (name == QStringLiteral("通道8")) {
        index = 7;
        myApp::VideoType = "0_7";
    } else if (name == QStringLiteral("通道9")) {
        index = 8;
        myApp::VideoType = "0_8";
    } else if (name == QStringLiteral("通道10")) {
        index = 9;
        myApp::VideoType = "0_9";
    } else if (name == QStringLiteral("通道11")) {
        index = 10;
        myApp::VideoType = "0_10";
    } else if (name == QStringLiteral("通道12")) {
        index = 11;
        myApp::VideoType = "0_11";
    }
    /*else if (name == QStringLiteral("通道13")) {
        index = 12;
    } else if (name == QStringLiteral("通道14")) {
        index = 13;
    } else if (name == QStringLiteral("通道15")) {
        index = 14;
    } else if (name == QStringLiteral("通道16")) {
        index = 15;
    }*/
    if(this->video_type_!=myApp::VideoType){
        this->video_type_=myApp::VideoType;
        change_video_1(index);
        myApp::WriteConfig();
    }else {
        qDebug()<<"Is aleardy in this model ,no change need.";
    }

}

void frmMain::change_video_1(int index)
{
    if(index>=0&&index<12){
        removelayout();
        video_max_=true;
        ui->gridLayout->addWidget(video_labs_.at(index),0,0);
        video_labs_.at(index)->setVisible(true);
    }else {
        qDebug()<<"Please give right input!!!";
    }

}

void frmMain::show_video_4()
{
    video_max_ = false;
    int index = 0;
    QAction *action = qobject_cast<QAction *>(sender());
    QString name = action->text();
    if (name == QStringLiteral("通道1-通道4")) {
        index = 0;
        myApp::VideoType = "1_4";
    } else if (name == QStringLiteral("通道5-通道8")) {
        index = 4;
        myApp::VideoType = "5_8";
    } else if (name == QStringLiteral("通道9-通道12")) {
        index = 8;
        myApp::VideoType = "9_12";
    }
    /*else if (name == QStringLiteral("通道13-通道16")) {
        index = 12;
        myApp::VideoType = "13_16";
    }*/
    if(this->video_type_!=myApp::VideoType){
        this->video_type_=myApp::VideoType;
        change_video_4(index);
        myApp::WriteConfig();
    }else {
        qDebug()<<"Is aleardy in this model ,no change need.";
    }
}

void frmMain::change_video_4(int index)
{
    if(index>=0&&index<9){
        removelayout();
        ui->gridLayout->addWidget(video_labs_.at(index+0),0,0);
        ui->gridLayout->addWidget(video_labs_.at(index+1),0,1);
        ui->gridLayout->addWidget(video_labs_.at(index+2),1,0);
        ui->gridLayout->addWidget(video_labs_.at(index+3),1,1);
        for(int i=index;i<index+4;++i){
            video_labs_.at(i)->setVisible(true);
        }
    }else{
        qDebug()<<"Please give right input!!!";
    }
}
void frmMain::show_video_6()
{
    video_max_ = false;
    int index = 0;
    QAction *action = qobject_cast<QAction *>(sender());
    QString name = action->text();
    if (name == QStringLiteral("通道1-通道6")) {
        index = 0;
        myApp::VideoType = "1_6";
    } else if (name == QStringLiteral("通道7-通道12")) {
        index = 6;
        myApp::VideoType = "7_12";
    }
    /*else if (name == QStringLiteral("通道13-通道16")) {
        index = 12;
        myApp::VideoType = "13_16";
    }*/
    if(this->video_type_!=myApp::VideoType){
        this->video_type_=myApp::VideoType;
        change_video_6(index);
        myApp::WriteConfig();
    }else {
        qDebug()<<"Is aleardy in this model ,no change need.";
    }

}
void frmMain::change_video_6(int index){
    if(index>=0&&index<7){
        removelayout();
        ui->gridLayout->addWidget(video_labs_.at(index+0),0,0,1,2);
        ui->gridLayout->addWidget(video_labs_.at(index+1),0,2,1,2);
        ui->gridLayout->addWidget(video_labs_.at(index+2),1,0,1,2);
        ui->gridLayout->addWidget(video_labs_.at(index+3),1,2,1,2);
        ui->gridLayout->addWidget(video_labs_.at(index+4),2,0,1,2);
        ui->gridLayout->addWidget(video_labs_.at(index+5),2,2,1,2);
        for(int i=index;i<index+6;++i){
            video_labs_.at(i)->setVisible(true);
        }
    }else{
        qDebug()<<"Please give right input!!!";
    }
}
void frmMain::show_video_7()
{
    video_max_ = false;
    int index = 0;
    QAction *action = qobject_cast<QAction *>(sender());
    QString name = action->text();
    if (name == QStringLiteral("通道1-通道7")) {
        index = 0;
        myApp::VideoType = "1_7";
    } else if (name == QStringLiteral("通道6-通道12")) {
        index = 5;
        myApp::VideoType = "6_12";
    }
    if(this->video_type_!=myApp::VideoType){
        this->video_type_=myApp::VideoType;
        change_video_7(index);
        myApp::WriteConfig();
    }else {
        qDebug()<<"Is aleardy in this model ,no change need.";
    }
}

void frmMain::change_video_7(int index)
{
    if(index>=0&&index<6){
        removelayout();
        //设置布局
        ui->gridLayout->addWidget(video_labs_.at(index+0),0,0,2,3);
        ui->gridLayout->addWidget(video_labs_.at(index+1),0,3,1,1);
        ui->gridLayout->addWidget(video_labs_.at(index+2),1,3,1,1);
        ui->gridLayout->addWidget(video_labs_.at(index+3),2,3,1,1);
        ui->gridLayout->addWidget(video_labs_.at(index+4),2,0,1,1);
        ui->gridLayout->addWidget(video_labs_.at(index+5),2,1,1,1);
        ui->gridLayout->addWidget(video_labs_.at(index+6),2,2,1,1);
        for (int i = 0; i < 7; ++i) {
            video_labs_.at(index+i)->setVisible(true);
        }
    }else{
        qDebug()<<"Please give right input!!!";
    }
}
void frmMain::show_video_12()
{
    video_max_ = false;
    myApp::VideoType = "1_12";
    if(this->video_type_!=myApp::VideoType){
        this->video_type_=myApp::VideoType;
        change_video_12();
        myApp::WriteConfig();
    }else {
        qDebug()<<"Is aleardy in this model ,no change need.";
    }
}

void frmMain::change_video_12()
{
    //删除原有布局
    removelayout();
    //重新设置布局
    change_video(0,3,4);
}
void frmMain::change_video(int index, int v_row,int v_col)
{
    int count = 0;
    int row = 0;
    int column = 0;
    //videoMax = false;
    for (int i = index; i < video_count_; ++i) {
            ui->gridLayout->addWidget(video_labs_.at(i), row, column);
            video_labs_.at(i)->setVisible(true);
            //widgets.at(i)->VideoPlay();//播放视频
            count++;
            column++;
            //到达行界限就开始下一行
            if (column == v_col) {
                row++;
                column = 0;
            }

        if (count == (v_row * v_col)) {
            break;
        }
    }
}
QString frmMain::GetNVRID(QString NVRIP)
{
    QSqlQuery query;
    QString sql = QStringLiteral("select [NVRID] from [NVRInfo]");
    sql += " where [NVRIP]='" + NVRIP + "'";
    query.exec(sql);
    query.next();
    return query.value(0).toString();
}

void frmMain::GetRtspAddr(QString NVRID, QString IPCIP, QString &IPCRtspAddrMain, QString &IPCRtspAddrSub)
{
    QSqlQuery query;
    QString sql = QStringLiteral("select [IPCRtspAddrMain],[IPCRtspAddrSub] from [IPCInfo] where [IPCUse]='启用'");
    sql += " and [NVRID]='" + NVRID + "'";
    query.exec(sql);
    while(query.next()) {
        //取出子码流地址,看是否IP相同
        QString rtspAddr = query.value(0).toString();
        QStringList temp = rtspAddr.split("/");
        QString ip = temp[2].split(":")[0];
        if (ip == IPCIP) {
            IPCRtspAddrMain = query.value(0).toString();
            IPCRtspAddrSub = query.value(1).toString();
            break;
        }
    }
}

void frmMain::on_treeMain_doubleClicked(const QModelIndex &index)
{
    //选中的是NVR则不处理
    if (ui->treeMain->currentItem()->parent() == 0) {
        return;
    }
    //当前还没有选择通道
    if (tempLab == 0) {
        return;
    }

    //取出双击摄像机的子码流地址
    //取出NVR编号及IPCID
    QString txt = ui->treeMain->currentItem()->parent()->text(0);
    QString NVRIP = txt.split("[")[1].split("]")[0];
    QString NVRID = GetNVRID(NVRIP);
    QString temp = ui->treeMain->currentIndex().data().toString();
    QString IPCIP = temp.split("[")[1].split("]")[0];

    //根据NVR编号和IP地址查询出该摄像机的子码流
    QString rtspAddr;
    QString IPCRtspAddrMain;
    QString IPCRtspAddrSub;
    GetRtspAddr(NVRID, IPCIP, IPCRtspAddrMain, IPCRtspAddrSub);
    rtspAddr = (myApp::RtspType == 0 ? IPCRtspAddrMain : IPCRtspAddrSub);

    //如果该摄像机不在线
    if (!myHelper::IPCEnable(rtspAddr)) {
        myHelper::ShowMessageBoxError(QStringLiteral("该摄像机不在线!"));
        return;
    }

    QString tempCH = tempLab->text();
    for (int i = 0; i < 12; i++) {
        if (video_labs_[i]->text() == tempCH) {
            ChangeRtspAddr(i, rtspAddr);
            myApp::WriteConfig();
            break;
        }
    }
}

void frmMain::screen_full()
{
    this->setGeometry(qApp->desktop()->geometry());
    this->layout()->setContentsMargins(0, 0, 0, 0);
    ui->widget_main->layout()->setContentsMargins(0, 0, 0, 0);
    ui->widget_title->setVisible(false);
    ui->treeMain->setVisible(false);
}

void frmMain::screen_normal()
{
    this->setGeometry(qApp->desktop()->availableGeometry());
    this->layout()->setContentsMargins(1, 1, 1, 1);
    ui->widget_main->layout()->setContentsMargins(5, 5, 5, 5);
    ui->widget_title->setVisible(true);
    ui->treeMain->setVisible(true);
}

void frmMain::on_labStart_linkActivated(const QString &link)
{

}

void frmMain::on_labConfig_linkActivated(const QString &link)
{

}

void frmMain::on_tab_choose_currentChanged(int index)
{
    if(index==2){
        //移除原来的video_control_widget_
//        video_vbox_layout_->removeWidget(video_control_widget_);
//        //ui->treeMain->setVisible(false);//隐藏侧边面板
//        //移除地图
//        map_vbox_layout_->removeWidget(map_control_widget_);
//        //清除综合模式布局
//        aggregative_gridLayout_->removeWidget(video_control_widget_);
//        aggregative_gridLayout_->removeWidget(map_control_widget_);
        aggregative_gridLayout_->removeWidget(speed_chart_widget_);
        //添加布局
        ui->treeMain->setVisible(false);//隐藏侧边面板
        aggregative_gridLayout_->addWidget(video_control_widget_,0,0,2,2);
        aggregative_gridLayout_->addWidget(map_control_widget_,0,2,3,2);
        aggregative_gridLayout_->addWidget(speed_chart_widget_,2,0,1,2);
    }else if(index==0){//选择了地图
        //地图还原
        map_vbox_layout_->addWidget(map_control_widget_);
    }else if(index==1){
        //视频控制还原
        ui->treeMain->setVisible(true);
        video_vbox_layout_->addWidget(video_control_widget_);
    }

}

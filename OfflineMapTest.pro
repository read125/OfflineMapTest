TEMPLATE = app
QT += qml  quick
QT += location positioning
QT += core gui
QT += network widgets
QT += multimedia multimediawidgets #add video and music
QT += quickwidgets
QT += network  #add network
QT += charts
QT += qml-private quick-private gui-private core-private
CONFIG += c++11

SOURCES += main.cpp \
    src/tool.cpp \
    src/busstation.cpp \
    src/screencontrol.cpp \
    src/busline.cpp \
    src/bus.cpp \
    src/mapcontrlconnect.cpp \
    src/mainshowdialog.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

include(QtLocationPlugin/LocationPlugin.pri)
include(test/test.pri)
include(VideoControl/VideoControl.pri)
include(player/player.pri)
#add ffmpeg and QtAV
#windows
win32 {
INCLUDEPATH += $$PWD/3rdpart/ffmpeg/include

LIBS += $$PWD/3rdpart/ffmpeg/lib/avcodec.lib \
        $$PWD/3rdpart/ffmpeg/lib/avdevice.lib \
        $$PWD/3rdpart/ffmpeg/lib/avfilter.lib \
        $$PWD/3rdpart/ffmpeg/lib/avformat.lib \
        $$PWD/3rdpart/ffmpeg/lib/avutil.lib \
        $$PWD/3rdpart/ffmpeg/lib/postproc.lib \
        $$PWD/3rdpart/ffmpeg/lib/swresample.lib \
        $$PWD/3rdpart/ffmpeg/lib/swscale.lib

INCLUDEPATH += $$PWD/3rdpart/QtAV/include


LIBS += $$PWD/3rdpart/QtAV/lib/Qt5AVd.lib \
        $$PWD/3rdpart/QtAV/lib/Qt5AVWidgetsd.lib \
        $$PWD/3rdpart/QtAV/lib/QtAVd1.lib \
        $$PWD/3rdpart/QtAV/lib/QtAVWidgetsd1.lib


}
#linux
unix:!macx: {
    FFMPEGDIR=$$PWD/3rdpart/ffmpeg-linux

    INCLUDEPATH += $$FFMPEGDIR/include

    LIBS += -L$$FFMPEGDIR/lib -lavcodec \
                          -lavfilter \
                          -lavformat  \
                          -lavresample \
                          -lavutil \
                          -lpostproc \
                          -lswresample \
                          -lswscale

    INCLUDEPATH += $$PWD/3rdpart/QtAV-linux/include

    LIBS += -L$$PWD/3rdpart/QtAV-linux/lib/ -lQmlAV -lQtAV -lQtAVWidgets

}


INCLUDEPATH += \
    QtLocationPlugin \
#地图相关插件
LOCATION_PLUGIN_DESTDIR = $${OUT_PWD}/QtLocationPlugin
LOCATION_PLUGIN_NAME    = GeoServiceProviderFactory

HEADERS += \
    src/tool.h \
    src/screencontrol.h \
    src/busstation.h \
    src/busline.h \
    src/bus.h \
    src/mapcontrlconnect.h \
    src/mainshowdialog.h

DISTFILES +=

FORMS += \
    src/mainshowdialog.ui


MOC_DIR         = temp/moc
RCC_DIR         = temp/rcc
UI_DIR          = temp/ui
OBJECTS_DIR     = temp/obj
DESTDIR         = bin

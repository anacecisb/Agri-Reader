



#-------------------------------------------------
#
# Project created by QtCreator 2016-02-19T11:02:25
#
#-------------------------------------------------

QT       += core gui serialport bluetooth uitools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QuickChips
TEMPLATE = app

#DEFINES += -USE_V4L

INCLUDEPATH += /usr/include/

SOURCES += main.cpp\
    quickchipsmainwindow.cpp \
    qtcamsrc/v4l2-api.cpp \
    qtcamsrc/videoencoder.cpp \
    qtcamsrc/videostreaming.cpp \
    qtcamsrc/h264decoder.cpp \
    IVDCore/kl46manager.cpp \
    IVDCore/btmanager.cpp \
    IVDCore/kl46system.cpp \
    IVDCore/roimanager.cpp \
    IVDCore/roisystem.cpp \
    IVDCore/roi.cpp \
    IVDCore/ivdimage.cpp \
    qtteardropwindow.cpp \    
    IVDCore/ProcessManager/qcprocessmanager.cpp \
    IVDCore/ProcessManager/processcommand.cpp \
    qcresultsmanager.cpp \
    qcassayresult.cpp \
    IVDCore/ivdtimehelper.cpp

HEADERS  += quickchipsmainwindow.h \
    qtcamsrc/v4l2-api.h \
    qtcamsrc/videoencoder.h \
    qtcamsrc/videostreaming.h \
    qtcamsrc/v4l2headers/include/linux/dvb/audio.h \
    qtcamsrc/v4l2headers/include/linux/dvb/dmx.h \
    qtcamsrc/v4l2headers/include/linux/dvb/frontend.h \
    qtcamsrc/v4l2headers/include/linux/dvb/video.h \
    qtcamsrc/v4l2headers/include/linux/compiler.h \
    qtcamsrc/v4l2headers/include/linux/fb.h \
    qtcamsrc/v4l2headers/include/linux/ivtv.h \
    qtcamsrc/v4l2headers/include/linux/v4l2-common.h \
    qtcamsrc/v4l2headers/include/linux/v4l2-controls.h \
    qtcamsrc/v4l2headers/include/linux/v4l2-mediabus.h \
    qtcamsrc/v4l2headers/include/linux/v4l2-subdev.h \
    qtcamsrc/v4l2headers/include/linux/videodev2.h \
    qtcamsrc/h264decoder.h \
    kl46manager.h \
    IVDCore/kl46manager.h \
    IVDCore/btmanager.h \
    IVDCore/kl46system.h \
    QCTimingMaster.h \
    IVDCore/roimanager.h \
    IVDCore/roisystem.h \
    IVDCore/roi.h \
    IVDCore/ivdimage.h \
    qtteardropwindow.h \    
    IVDCore/ProcessManager/qcprocessmanager.h \
    IVDCore/ProcessManager/processcommand.h \
    qcresultsmanager.h \
    qcassayresult.h \
    IVDCore/ivdtimehelper.h

LIBS += -lv4l2 \
        -lv4lconvert \
        -lavutil \
        -lavcodec \
        -lavformat \
        -lswscale \
        -ludev \
        -lusb-1.0 \
        -L/usr/lib \
        -lapm \
        -lturbojpeg

FORMS    += \
    quickchipsmainwindow.ui \
    qtteardropwindow.ui

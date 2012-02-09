#-------------------------------------------------
#
# Project created by QtCreator 2012-02-07T14:19:21
#
#-------------------------------------------------

QT       += core gui

TARGET = DrawingBenchmark
TEMPLATE = app


SOURCES += main.cpp\
        smainwnd.cpp \
    SDrawingView.cpp \
    SDrawingScene.cpp \
    SSettingsWidget.cpp

HEADERS  += smainwnd.h \
    SGlobals.h \
    SDrawingView.h \
    SDrawingScene.h \
    SSettingsWidget.h

RESOURCES += \
    res.qrc

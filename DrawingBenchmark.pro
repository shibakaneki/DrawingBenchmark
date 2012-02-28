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
    SSettingsWidget.cpp \
    SColorWidget.cpp \
    SBrushPropertiesWidget.cpp \
    SDockPalette.cpp \
    STopicTitleLabel.cpp \
    SSlider.cpp \
    SGraphicsPathItem.cpp \
    SDrawingWidget.cpp \
    SGraphicsItemGroup.cpp

HEADERS  += smainwnd.h \
    SGlobals.h \
    SDrawingView.h \
    SDrawingScene.h \
    SSettingsWidget.h \
    SColorWidget.h \
    SBrushPropertiesWidget.h \
    SDockPalette.h \
    STopicTitleLabel.h \
    SSlider.h \
    SGraphicsPathItem.h \
    SDrawingWidget.h \
    SGraphicsItemGroup.h

RESOURCES += \
    res.qrc

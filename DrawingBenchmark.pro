# -------------------------------------------------
# Project created by QtCreator 2012-02-07T14:19:21
# -------------------------------------------------
QT += core \
    gui
TARGET = DrawingBenchmark
TEMPLATE = app

INCLUDEPATH += src
include(src/core/core.pri)
include(src/dockpalettes/dockpalettes.pri)

SOURCES += src/SCatmullRomSpline.cpp \
    src/SCubicSpline.cpp \
    src/SDrawingScene.cpp \
    src/SDrawingView.cpp \
    src/SDrawingWidget.cpp \
    src/SGraphicsItem.cpp \
    src/SGraphicsItemGroup.cpp \
    src/SGraphicsPathItem.cpp \
    src/SGraphicsPictureItem.cpp \
    src/SSelectionRect.cpp \
    src/SSlider.cpp \
    src/SStrokeItem.cpp \
    src/STopicTitleLabel.cpp \
    src/main.cpp \
    src/smainwnd.cpp
HEADERS += src/SCatmullRomSpline.h \
    src/SCubicSpline.h \
    src/SDrawingScene.h \
    src/SDrawingView.h \
    src/SDrawingWidget.h \
    src/SGraphicsItem.h \
    src/SGraphicsItemGroup.h \
    src/SGraphicsPathItem.h \
    src/SGraphicsPictureItem.h \
    src/SSelectionRect.h \
    src/SSlider.h \
    src/SStrokeItem.h \
    src/STopicTitleLabel.h \
    src/smainwnd.h
RESOURCES += res.qrc

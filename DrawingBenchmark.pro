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
include(src/graphicsitems/graphicsitems.pri)
include(src/maths/maths.pri)
include(src/workspace/workspace.pri)
include(src/customwidgets/customwidgets.pri)
SOURCES += src/main.cpp
HEADERS += 
RESOURCES += res.qrc

QMAKE_CXXFLAGS_DEBUG += -pg
QMAKE_LFLAGS_DEBUG += -pg
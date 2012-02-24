#ifndef SGLOBALS_H
#define SGLOBALS_H

#include <QDebug>
#include <QString>
#include <QFile>

//#define ENABLE_DEBUG    1
//#define REALTIME_INTERPOLATION  1

#define DELETEPTR(ptr) if(NULL != ptr){ \
    delete ptr; \
    ptr = NULL; \
    }

#define SETUP_STYLESHEET { \
    QString style; \
    QFile f(":/style.qss"); \
    if(f.exists()) \
    { \
        if(f.open(QIODevice::ReadOnly)) \
        { \
            style = QString(f.readAll()); \
            setStyleSheet(style); \
        } \
    } \
}

typedef enum{
    eTool_Arrow,
    eTool_Pen,
    eTool_Eraser,
    eTool_ZoomIn,
    eTool_ZoomOut,
    eTool_Pan
}eTool;

#endif // SGLOBALS_H

#ifndef SGLOBALS_H
#define SGLOBALS_H

#include <QDebug>
#include <QString>
#include <QFile>

// Modes
//#define ENABLE_DEBUG    1
#define REALTIME_INTERPOLATION  1

// Sizes
#define GRIPSIZE    6
#define SELECTION_WIDTH 1

// Strings
#define ON_SETTINGS_LABEL		"SSettingsLabel"
#define ON_PALETTE_CONTAINER	"SPaletteContainer"
#define ON_COLOR_PREVIEW		"SColorPreview"
#define ON_LEAFWIDGET			"SLeafWidget"

// Others
#define PI (4.0 * atan(1.0))

// Macros
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

// Types
typedef enum{
    eTool_Arrow,
    eTool_Pen,
    eTool_Eraser,
    eTool_ZoomIn,
    eTool_ZoomOut,
    eTool_Pan
}eTool;

typedef struct{
    float x;
    float y;
    qreal lineWidth;
    qreal rotation;
    int xTilt;
    int ytilt;
}sPoint;

typedef struct{
	sPoint p1;
	sPoint p2;
}sLine;

#endif // SGLOBALS_H

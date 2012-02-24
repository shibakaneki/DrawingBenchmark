#ifndef SDRAWINGWIDGET_H
#define SDRAWINGWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QMouseEvent>
#include <QTabletEvent>

#include "SGlobals.h"

class SDrawingWidget : public QWidget
{
    Q_OBJECT
public:
    SDrawingWidget(QWidget* parent=0, const char* name="SDrawingWidget");
    ~SDrawingWidget();

protected:
    void paintEvent(QPaintEvent* ev);
    void mousePressEvent(QMouseEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);
    void tabletEvent(QTabletEvent* ev);

private:
    QPen mPen;
    qreal mLineWidth;
    int mRed;
    int mGreen;
    int mBlue;
    int mAlpha;
    qreal mPressure;
    qreal mRotation;
    int mXTilt;
    int mYTilt;
};

#endif // SDRAWINGWIDGET_H

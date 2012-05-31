#include "SDrawingWidget.h"

SDrawingWidget::SDrawingWidget(QWidget *parent, const char *name):QWidget(parent)
{
    setObjectName(name);
    mPressure = 1.0;
    mRotation = 0.0;
    mXTilt = 0;
    mYTilt = 0;
    mLineWidth = 3;
    mRed = 0;
    mGreen = 0;
    mBlue = 0;
    mAlpha = 255;
    mPen.setColor(QColor(mRed, mGreen, mBlue, mAlpha));

    // TODO: Finish the implementation of this widget
}

SDrawingWidget::~SDrawingWidget()
{

}

void SDrawingWidget::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev);
}

void SDrawingWidget::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);
}

void SDrawingWidget::mouseMoveEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);
}

void SDrawingWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev);
}

void SDrawingWidget::tabletEvent(QTabletEvent *ev)
{
    mPressure = ev->pressure();
    mRotation = ev->rotation();
    mXTilt = ev->xTilt();
    mYTilt = ev->yTilt();
    if(QTabletEvent::TabletPress == ev->type()){

    }else if(QTabletEvent::TabletMove == ev->type()){

    }else if(QTabletEvent::TabletRelease == ev->type()){

    }
    mPressure = 1.0;
    mXTilt = 0;
    mYTilt = 0;
    mRotation = 0.0;
}

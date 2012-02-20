#ifndef SDRAWINGVIEW_H
#define SDRAWINGVIEW_H

#include <QGraphicsView>
#include <QPointF>
#include <QMouseEvent>
#include <QVector>
#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QPen>
#include <QPainterPath>
#include <QTabletEvent>

#include "SGlobals.h"
#include "SDrawingScene.h"

typedef struct{
    QPointF p0;
    QPointF p1;
    QPointF c0;
    QPointF c1;
    QGraphicsRectItem* pOrigin;
    QGraphicsRectItem* pEndPoint;
    QGraphicsEllipseItem* pC0;
    QGraphicsEllipseItem* pC1;
    QGraphicsSimpleTextItem* pP0Label;
    QGraphicsSimpleTextItem* pP1Label;
    QGraphicsSimpleTextItem* pC0Label;
    QGraphicsSimpleTextItem* pC1Label;
}sSplineElement;

class SDrawingView : public QGraphicsView
{
    Q_OBJECT
public:
    SDrawingView(QWidget* parent=0, const char* name="SDrawingView");
    ~SDrawingView();
    void setSmoothFactor(int val){mSmoothFactor = val;}
    int smoothFactor(){return mSmoothFactor;}

public slots:
    void onSmoothnessChanged(int smoothFactor);
    void onClearPage();
    void onSetCurrentTool(eTool tool);
    void onPointSelected(QPointF p0, QPointF p1, QPointF c0, QPointF c1);
    void onLineWidthChanged(int w);
    void onColorChanged(const QColor& color);

signals:
    void currentPointChanged(QPointF p);
    void clearCoefficients();
    void addCoefficients(QPointF p0, QPointF p1, QPointF c0, QPointF c1);
    void zoomChanged(int zoomDepth);

protected:
    void mousePressEvent(QMouseEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);
    void tabletEvent(QTabletEvent* ev);

private:
    void draw(QPointF prev, QPointF crnt);
    void optimizeLines();
    void clearInfos();
    void addSplineInfos(QPointF p0, QPointF p1, QPointF c0, QPointF c1);
    double cosineInterpolate(double y1, double y2, double mu);
    double cubicInterpolate(double y0,double y1, double y2,double y3, double mu);
    double hermiteInterpolate(double y0,double y1, double y2,double y3, double mu, double tension, double bias);
    QPainterPath generatePath();
    QPainterPath basicSmoothing();
    QPainterPath lagrangeSmoothing();
    QPainterPath cosineSmoothing();
    QPainterPath cubicSmoothing();
    QPainterPath hermiteSmoothing();
    void performPressEvent(QPoint p);
    void performMoveEvent(QPoint p);
    void performReleaseEvent(QPoint p);

    SDrawingScene* mpScene;
    QPen mPen;
    bool mDrawingInProgress;
    QPointF mPreviousPos;
    QVector<QPointF> mPoints;
    QVector<QGraphicsLineItem*> mLines;
    QVector<QGraphicsItem*> mItems;
    eTool mCurrentTool;
    int mSmoothFactor;
    float mScaleFactor;
    QPointF mCurrentCenterPoint;
    int mRed;
    int mGreen;
    int mBlue;
    int mAlpha;
    QGraphicsItem* mpSelectedItem;
    int mPreviousSlope;
    int mZoomDepth;
    QPointF mPanFirstPoint;
    int mLineWidth;

    // Selection indicators
    QVector<sSplineElement> mSplines;
};

#endif // SDRAWINGVIEW_H

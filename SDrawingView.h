#ifndef SDRAWINGVIEW_H
#define SDRAWINGVIEW_H

#include <QTime>

#include <QGraphicsView>
#include <QPointF>
#include <QMouseEvent>
#include <QVector>
#include <QGraphicsLineItem>
#include <QGraphicsItemGroup>
#include <QGraphicsItem>
#include <QPen>
#include <QPainterPath>
#include <QTabletEvent>
#include <QPaintEvent>
#include <QRubberBand>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QKeyEvent>

#include "SGlobals.h"
#include "SDrawingScene.h"
#include "SGraphicsItemGroup.h"
#include "SGraphicsPictureItem.h"

typedef struct{
    float x;
    float y;
    qreal lineWidth;
    //qreal pressure;
    qreal rotation;
    int xTilt;
    int ytilt;
}sPoint;

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

class SRubberBand : public QRubberBand
{
    Q_OBJECT
public:
    SRubberBand(QWidget* parent=0, const char* name="SRubberBand");
    ~SRubberBand();

protected:
    void paintEvent(QPaintEvent* ev);

private:
    QColor mPenColor;
    QColor mBrushColor;
};

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
    void dragEnterEvent(QDragEnterEvent* ev);
    void dragMoveEvent(QDragMoveEvent* ev);
    void dragLeaveEvent(QDragLeaveEvent* ev);
    void dropEvent(QDropEvent* ev);
    void keyPressEvent(QKeyEvent* ev);

private:
    void draw(sPoint prev, sPoint crnt);
    void optimizeLines();
    void clearInfos();
    void addSplineInfos(QPointF p0, QPointF p1, QPointF c0, QPointF c1);
    double hermiteInterpolate(double y0,double y1, double y2,double y3, double mu, double tension, double bias);
    QPainterPath generatePath();
    QPainterPath hermiteSmoothing();
    QPainterPath catmullRomSmoothing();

    void performPressEvent(QPoint p);
    void performMoveEvent(QPoint p);
    void performReleaseEvent(QPoint p);

    SDrawingScene* mpScene;
    QPen mPen;
    bool mDrawingInProgress;
    sPoint mPreviousPos;
    QVector<sPoint> mPoints;
    QVector<QGraphicsItem*> mLines;
    QVector<QGraphicsItem*> mItems;
    eTool mCurrentTool;
    int mSmoothFactor;
    float mScaleFactor;
    QPointF mCurrentCenterPoint;
    int mRed;
    int mGreen;
    int mBlue;
    int mAlpha;
    QVector<QGraphicsItem*> mSelectedItems;
    int mPreviousSlope;
    int mZoomDepth;
    QPointF mPanFirstPoint;
    int mLineWidth;
    int mNextZValue;
    QPointF mSelectedCurrentPoint;
    qreal mPressure;
    qreal mRotation;
    int mXTilt;
    int mYTilt;
    QVector<sSplineElement> mSplines;
    SRubberBand* mpRubber;
    QPoint mSelectionOrigin;
    bool mSelectionInProgress;
    SGraphicsPictureItem* mpTmpPictureItem;
    bool mResizeInProgress;
    eGrip mCrntGrip;
};

#endif // SDRAWINGVIEW_H

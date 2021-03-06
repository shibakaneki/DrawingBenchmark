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

#include "core/SGlobals.h"
#include "SDrawingScene.h"
#include "graphicsitems/SGraphicsItemGroup.h"
#include "graphicsitems/SGraphicsPictureItem.h"
#include "graphicsitems/SStrokeItem.h"
#include "graphicsitems/SStroke.h"
#include "drawing/SBrush.h"
#include "drawing/SDrawingController.h"

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
    void clearPage();
    void performDrawingJob(eInputType input);

public slots:
    void onSmoothnessChanged(int smoothFactor);
    void onBrushChanged(SBrush* b);

signals:
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
    void drawCurrentLine();

    void performPressEvent(QPoint p);
    void performMoveEvent(QPoint p);
    void performReleaseEvent(QPoint p);
    sPoint* updatePenParameters(QPoint point);

    SDrawingScene* mpScene;
    QPen mPen;
    bool mDrawingInProgress;
    sPoint mPreviousPos;
    QVector<sPoint> mPoints;
    QVector<QGraphicsItem*> mLines;
    QVector<QGraphicsItem*> mItems;
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
    SStrokeItem* mpCurrentStroke;
    SStroke* mCurrentStroke;
    SDrawingController* mpDrawingController;
};

#endif // SDRAWINGVIEW_H

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

#include "SGlobals.h"
#include "SDrawingScene.h"

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

signals:
    void currentPointChanged(QPointF p);

protected:
    void mousePressEvent(QMouseEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);

private:
    void draw(QPointF prev, QPointF crnt);
    void optimizeLines();
    QPainterPath generatePath();
    QPainterPath basicSmoothing();
    QPainterPath lagrangeSmoothing();

    SDrawingScene* mpScene;
    QPen mPen;
    bool mDrawingInProgress;
    QPointF mPreviousPos;
    QVector<QPointF> mPoints;
    QVector<QGraphicsLineItem*> mLines;
    QVector<QGraphicsItem*> mItems;
    eTool mCurrentTool;
    int mSmoothFactor;
    unsigned int mRed;
    unsigned int mGreen;
    unsigned int mBlue;
    QGraphicsItem* mpSelectedItem;
};

#endif // SDRAWINGVIEW_H

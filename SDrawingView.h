#ifndef SDRAWINGVIEW_H
#define SDRAWINGVIEW_H

#include <QGraphicsView>
#include <QPointF>
#include <QMouseEvent>
#include <QVector>
#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QPen>

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

signals:
    void currentPointChanged(QPointF p);

protected:
    void mousePressEvent(QMouseEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);

private:
    void draw(QPointF prev, QPointF crnt);
    void optimizeLines();

    SDrawingScene* mpScene;

    QPen mPen;
    bool mDrawingInProgress;
    QPointF mPreviousPos;
    QVector<QPointF> mPoints;
    QVector<QGraphicsLineItem*> mLines;
    QVector<QGraphicsItem*> mItems;

    // Settings
    int mSmoothFactor;
};

#endif // SDRAWINGVIEW_H

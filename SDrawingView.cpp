#include <QPainterPath>

#include "SDrawingView.h"

SDrawingView::SDrawingView(QWidget *parent, const char *name):QGraphicsView(parent)
  , mpScene(NULL)
{
    setObjectName(name);
    setStyleSheet("background:white;");
    setRenderHint(QPainter::Antialiasing, true);
    mpScene = new SDrawingScene(this);
    setScene(mpScene);
    mItems.clear();

    mPen.setColor(Qt::red);
    mPen.setWidth(5);
}

SDrawingView::~SDrawingView()
{
    DELETEPTR(mpScene);
}

void SDrawingView::mousePressEvent(QMouseEvent *ev)
{
    QPointF mappedPoint = mapToScene(ev->pos());
    emit currentPointChanged(mappedPoint);
    mDrawingInProgress = true;
    mPoints.clear();
    mLines.clear();
    mPreviousPos = mappedPoint;
    mPoints << mPreviousPos;
}

void SDrawingView::mouseMoveEvent(QMouseEvent *ev)
{
    QPointF mappedPoint = mapToScene(ev->pos());
    emit currentPointChanged(mappedPoint);
    if(mDrawingInProgress){
        draw(mPreviousPos, mappedPoint);
        mPreviousPos = mappedPoint;
        mPoints << mappedPoint;
    }
}

void SDrawingView::mouseReleaseEvent(QMouseEvent *ev)
{
    QPointF mappedPoint = mapToScene(ev->pos());
    emit currentPointChanged(mappedPoint);
    draw(mPreviousPos, mappedPoint);
    mDrawingInProgress = false;
    mPoints << mappedPoint;

    // Refine the strokes
    optimizeLines();
}

void SDrawingView::draw(QPointF prev, QPointF crnt)
{
    if(NULL != mpScene){
        mLines << mpScene->addLine(prev.x(), prev.y(), crnt.x(), crnt.y(), mPen);
    }
}

void SDrawingView::optimizeLines()
{
    foreach(QGraphicsLineItem* line, mLines){
        mpScene->removeItem(line);
    }

    QPainterPath path;
    foreach(QPointF p, mPoints){
        path.lineTo(p);
    }
    mItems << mpScene->addPath(path, mPen);
}

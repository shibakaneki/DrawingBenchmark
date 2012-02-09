#include <QPainterPath>

#include "SDrawingView.h"

SDrawingView::SDrawingView(QWidget *parent, const char *name):QGraphicsView(parent)
  , mpScene(NULL)
{
    setObjectName(name);
    setStyleSheet("background:white;");
    setRenderHint(QPainter::Antialiasing, true);
    mpScene = new SDrawingScene(this);
    mpScene->setSceneRect(rect());
    setScene(mpScene);
    mItems.clear();

    mPen.setColor(Qt::blue);
    mPen.setWidth(5);
    mPen.setCapStyle(Qt::RoundCap);
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

    // At least 2 points for a line!
    if(mPoints.size() >= 2){
        for(int i=0; i<mPoints.size()-1; i++){
            if(0 == i){
                path.moveTo(mPoints.at(i));
            }else if(i+2 < mPoints.size()){
                path.cubicTo(mPoints.at(i), mPoints.at(i+1), mPoints.at(i+2));
                //path.quadTo(mPoints.at(i), mPoints.at(i+1));
            }
        }
    }

    mItems << mpScene->addPath(path, mPen);
}

void SDrawingView::onSmoothnessChanged(int smoothFactor)
{
    mSmoothFactor = smoothFactor;
}

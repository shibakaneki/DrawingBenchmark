#include <QDebug>
#include <QPainterPath>

#include "SDrawingView.h"

SDrawingView::SDrawingView(QWidget *parent, const char *name):QGraphicsView(parent)
  , mpScene(NULL)
  , mpSelectedItem(NULL)
{
    setObjectName(name);
    setStyleSheet("background:white;");
    setRenderHint(QPainter::Antialiasing, true);
    mpScene = new SDrawingScene(this);
    mpScene->setSceneRect(rect());
    setScene(mpScene);
    mItems.clear();

    mRed = 0;
    mGreen = 0;
    mBlue = 255;
    mCurrentTool = eTool_Pen;

    mPen.setColor(QColor(mRed, mGreen, mBlue));
    mPen.setWidth(3);
    mPen.setCapStyle(Qt::RoundCap);
}

SDrawingView::~SDrawingView()
{
    DELETEPTR(mpScene);
}

void SDrawingView::mousePressEvent(QMouseEvent *ev)
{
    if(eTool_Pen == mCurrentTool){
        QPointF mappedPoint = mapToScene(ev->pos());
        emit currentPointChanged(mappedPoint);
        mDrawingInProgress = true;
        mPoints.clear();
        mLines.clear();
        mPreviousPos = mappedPoint;
        mPoints << mPreviousPos;
    }else if(eTool_Arrow){
        QGraphicsItem* pItem = itemAt(ev->pos());
        if(NULL != pItem){
            if(NULL != mpSelectedItem && mpSelectedItem != pItem){
                mpSelectedItem->setSelected(false);
                mpSelectedItem = pItem;
                mpSelectedItem->setSelected(true);

                // TODO: add some indication saying that the item is selected.

            }
        }
    }
}

void SDrawingView::mouseMoveEvent(QMouseEvent *ev)
{
    if(eTool_Pen == mCurrentTool){
        QPointF mappedPoint = mapToScene(ev->pos());
        emit currentPointChanged(mappedPoint);
        if(mDrawingInProgress){
            draw(mPreviousPos, mappedPoint);
            mPreviousPos = mappedPoint;
            mPoints << mappedPoint;
        }
    }else if(eTool_Arrow == mCurrentTool){
        if(NULL != mpSelectedItem){
            // Move the item

        }
    }
}

void SDrawingView::mouseReleaseEvent(QMouseEvent *ev)
{
    if(eTool_Pen == mCurrentTool){
        QPointF mappedPoint = mapToScene(ev->pos());
        emit currentPointChanged(mappedPoint);
        draw(mPreviousPos, mappedPoint);
        mDrawingInProgress = false;
        mPoints << mappedPoint;

        // Refine the strokes
        optimizeLines();
    }
}

void SDrawingView::draw(QPointF prev, QPointF crnt)
{
    if(NULL != mpScene){
        mLines << mpScene->addLine(prev.x(), prev.y(), crnt.x(), crnt.y(), mPen);
    }
}

void SDrawingView::optimizeLines()
{
    // Remove the individual lines
    foreach(QGraphicsLineItem* line, mLines){
        mpScene->removeItem(line);
    }

    /*
    mPoints.clear();
    mPoints << QPointF(-250, -150);
    mPoints << QPointF(300, 300);
    mPoints << QPointF(550, -300);
    mPoints << QPointF(400, -40);
    mPoints << QPointF(-100, 200);

    mRed += 15;
    mBlue -= 15;
    mGreen += 5;
    mPen.setColor(QColor(mRed, mGreen, mBlue));
    */

    // Generate a smooth path
    QPainterPath path = generatePath();

    // Add it as a single element to the scene
    mItems << mpScene->addPath(path, mPen);
}

QPainterPath SDrawingView::generatePath()
{
    return basicSmoothing();
    //return lagrangeSmoothing();
}

QPainterPath SDrawingView::lagrangeSmoothing()
{
    QPainterPath path;

    return path;
}

// This interpolation algorithm is the first one we did by using 3 points and tangents
QPainterPath SDrawingView::basicSmoothing()
{
    QPainterPath path;

    // -- First Point ---------------------
    // At least 2 points for a line!
    if(!mPoints.empty()){
        // Set the origin of the path
        path.moveTo(mPoints.at(0));
    }

    // -- Intermediate Points -------------
    for(int i=1; i<mPoints.size()-1; i++){
        QPointF origin = mPoints.at(i-1);
        QPointF endpoint = mPoints.at(i);
        QPointF next = mPoints.at(i+1);
        QPointF intersect;
        QPointF c1;
        QPointF c2;

        int dX = abs(next.x()-origin.x());
        if(dX < 1){
            path.lineTo(endpoint);
            continue;
        }

        // Get the tangents parameters
        float c2Slope = (next.y() - origin.y()) / (next.x() - origin.x());
        float c1Slope;
        if(1 == i){
            c1Slope = -1/c2Slope;
        }else{
            c1Slope = mPreviousSlope;
        }
        mPreviousSlope = c2Slope;
        int c2Offset = endpoint.y() - c2Slope*endpoint.x();
        int c1Offset = origin.y() - c1Slope*origin.x();

        // Get the tangents intersection point coordinates
        int dSlope = abs(c2Slope - c1Slope);
        if(dSlope < 1){
            path.lineTo(endpoint);
            continue;
        }
        int x = (c1Offset - c2Offset)/(c2Slope-c1Slope);
        int y = c2Slope*x + c2Offset;
        intersect.setX(x);
        intersect.setY(y);

        // Get the c1 coefficient related to the smooth factor
        int c1X = origin.x() + (int)((intersect.x() - origin.x())*((float)mSmoothFactor/100));
        int c1Y = origin.y() + (int)((intersect.y() - origin.y())*((float)mSmoothFactor/100));
        c1.setX(c1X);
        c1.setY(c1Y);

        // Get the c1 coefficient related to the smooth factor
        int c2X = endpoint.x() + (int)((intersect.x() - endpoint.x())*((float)mSmoothFactor/100));
        int c2Y = endpoint.y() + (int)((intersect.y() - endpoint.y())*((float)mSmoothFactor/100));
        c2.setX(c2X);
        c2.setY(c2Y);

        // Add the spline to the path
        path.cubicTo(c1, c2, endpoint);
    }

    // -- Last Point ----------------------


    return path;
}

void SDrawingView::onSmoothnessChanged(int smoothFactor)
{
    mSmoothFactor = smoothFactor;
}

void SDrawingView::onClearPage()
{
    mPoints.clear();
    mLines.clear();
    mItems.clear();
    mpScene->clear();
}

void SDrawingView::onSetCurrentTool(eTool tool)
{
    mCurrentTool = tool;
}

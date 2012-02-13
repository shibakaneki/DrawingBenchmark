#include <QDebug>
#include <QPainterPath>

#include "SDrawingView.h"

SDrawingView::SDrawingView(QWidget *parent, const char *name):QGraphicsView(parent)
  , mpScene(NULL)
  , mpSelectedItem(NULL)
  , mpP0(NULL)
  , mpP1(NULL)
  , mpC0(NULL)
  , mpC1(NULL)
  , mpP0Label(NULL)
  , mpP1Label(NULL)
  , mpC0Label(NULL)
  , mpC1Label(NULL)
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

    mSmoothFactor = 75;
    mScaleFactor = 1.15;
}

SDrawingView::~SDrawingView()
{
    DELETEPTR(mpP0Label);
    DELETEPTR(mpP1Label);
    DELETEPTR(mpC0Label);
    DELETEPTR(mpC1Label);
    DELETEPTR(mpP0);
    DELETEPTR(mpP1);
    DELETEPTR(mpC0);
    DELETEPTR(mpC1);
    DELETEPTR(mpSelectedItem);
    DELETEPTR(mpScene);
}

void SDrawingView::mousePressEvent(QMouseEvent *ev)
{
    if(eTool_Pen == mCurrentTool){
        // Draw
        QPointF mappedPoint = mapToScene(ev->pos());
        emit currentPointChanged(mappedPoint);
        mDrawingInProgress = true;
        mPoints.clear();
        mLines.clear();
        mPreviousPos = mappedPoint;
        mPoints << mPreviousPos;
    }else if(eTool_Arrow == mCurrentTool){
        // Select
        QGraphicsItem* pItem = itemAt(ev->pos());
        if(NULL != pItem){
            if(NULL != mpSelectedItem && mpSelectedItem != pItem){
                mpSelectedItem->setSelected(false);
                mpSelectedItem = pItem;
                mpSelectedItem->setSelected(true);

                // TODO: add some indication saying that the item is selected.

            }
        }
    }else if(eTool_ZoomIn == mCurrentTool){
        // Zoom In
        scale(mScaleFactor, mScaleFactor);
        centerOn(ev->posF());

    }else if(eTool_ZoomOut == mCurrentTool){
        // Zoom Out
        scale(1.0/mScaleFactor, 1.0/mScaleFactor);
        centerOn(ev->posF());
    }else if(eTool_Pan == mCurrentTool){
        // Pan

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
    }else if(eTool_Pan == mCurrentTool){
        // Pan

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
    }else if(eTool_Pan == mCurrentTool){
        // Pan

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

    emit clearCoefficients();
    clearInfos();

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

    // TODO: Implement me!

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

        emit addCoefficients(origin, endpoint, c1, c2);
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
    clearInfos();
    mPoints.clear();
    mLines.clear();
    mItems.clear();
    mpScene->clear();
}

void SDrawingView::onSetCurrentTool(eTool tool)
{
    mCurrentTool = tool;
}


void SDrawingView::onPointSelected(QPointF p0, QPointF p1, QPointF c0, QPointF c1)
{
    clearInfos();

    QPen pointPen;
    pointPen.setColor(Qt::green);
    QPen coeffPen;
    coeffPen.setColor(Qt::red);
    int w = 4;
    QFont font;

    mpP0 = mpScene->addRect(p0.x()-w, p0.y()-w, 2*w, 2*w, pointPen);
    mpP0Label = mpScene->addSimpleText("P0");
    mpP0Label->setPos(p0.x() + w +1, p0.y() + w + 1);
    mpP1 = mpScene->addRect(p1.x()-w, p1.y()-w, 2*w, 2*w, pointPen);
    mpP1Label = mpScene->addSimpleText("P1");
    mpP1Label->setPos(p1.x() + w +1, p1.y() + w + 1);
    mpC0 = mpScene->addEllipse(c0.x()-w, c0.y()-w, 2*w, 2*w, coeffPen);
    mpC0Label = mpScene->addSimpleText("C0");
    mpC0Label->setPos(c0.x() + w +1, c0.y() + w + 1);
    mpC1 = mpScene->addEllipse(c1.x()-w, c1.y()-w, 2*w, 2*w, coeffPen);
    mpC1Label = mpScene->addSimpleText("C1");
    mpC1Label->setPos(c1.x() + w +1, c1.y() + w + 1);
}

void SDrawingView::clearInfos()
{
    QVector<QGraphicsItem*> items;
    items << mpP0;
    items << mpP0Label;
    items << mpP1;
    items << mpP1Label;
    items << mpC0;
    items << mpC0Label;
    items << mpC1;
    items << mpC1Label;

    foreach(QGraphicsItem* it, items){
        if(NULL != it){
            mpScene->removeItem(it);
            //DELETEPTR(it);
        }
    }
}

#include <QDebug>
#include <QPainterPath>
#include <QApplication>
#include <QDesktopWidget>
#include <math.h>

#include "SDrawingView.h"

SDrawingView::SDrawingView(QWidget *parent, const char *name):QGraphicsView(parent)
  , mpScene(NULL)
  , mpSelectedItem(NULL)
{
    SETUP_STYLESHEET
    setObjectName(name);
    setStyleSheet("background:white;");
    setRenderHint(QPainter::Antialiasing, true);
    mpScene = new SDrawingScene(this);
    setScene(mpScene);
    mpScene->setSceneRect(QApplication::desktop()->rect());
    mItems.clear();
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);

    mRed = 0;
    mGreen = 0;
    mBlue = 0;
    mAlpha = 255;
    mCurrentTool = eTool_Pen;

    mPen.setColor(QColor(mRed, mGreen, mBlue,mAlpha));
    mPen.setWidth(3);
    mPen.setCapStyle(Qt::RoundCap);

    mSmoothFactor = 75;
    mScaleFactor = 1.50;
    mZoomDepth = 0;
    mDrawingInProgress = false;
}

SDrawingView::~SDrawingView()
{
    DELETEPTR(mpSelectedItem);
    DELETEPTR(mpScene);
}

void SDrawingView::mousePressEvent(QMouseEvent *ev)
{
    performPressEvent(ev->pos());
}

void SDrawingView::mouseMoveEvent(QMouseEvent *ev)
{
    performMoveEvent(ev->pos());
}

void SDrawingView::mouseReleaseEvent(QMouseEvent *ev)
{
    performReleaseEvent(ev->pos());
}

void SDrawingView::tabletEvent(QTabletEvent* ev)
{
    if(QTabletEvent::TabletPress == ev->type()){
        performPressEvent(ev->pos());
    }else if(QTabletEvent::TabletMove == ev->type()){
        performMoveEvent(ev->pos());
    }else if(QTabletEvent::TabletRelease == ev->type()){
        performReleaseEvent(ev->pos());
    }
}

void SDrawingView::performPressEvent(QPoint p)
{
    QPointF mappedPoint = mapToScene(p);
    emit currentPointChanged(mappedPoint);
    if(eTool_Pen == mCurrentTool){
        // Draw
        mDrawingInProgress = true;
        mPoints.clear();
        mLines.clear();
        clearInfos();
        mPreviousPos = mappedPoint;
        mPoints << mPreviousPos;
    }else if(eTool_Arrow == mCurrentTool){
        // Select
        QGraphicsItem* pItem = itemAt(p);
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
        mZoomDepth++;
        emit zoomChanged(mZoomDepth);
        scale(mScaleFactor, mScaleFactor);
        centerOn(mappedPoint);
    }else if(eTool_ZoomOut == mCurrentTool){
        // Zoom Out
        if(0 < mZoomDepth){
            mZoomDepth--;
            emit zoomChanged(mZoomDepth);
            scale(1.0/mScaleFactor, 1.0/mScaleFactor);
            centerOn(mappedPoint);
        }

    }else if(eTool_Pan == mCurrentTool){
        // Pan
        mPanFirstPoint = mappedPoint;
    }
}

void SDrawingView::performMoveEvent(QPoint p)
{
    QPointF mappedPoint = mapToScene(p);
    emit currentPointChanged(mappedPoint);
    if(eTool_Pen == mCurrentTool){
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
        int centerX = viewport()->rect().width()/2;
        int centerY = viewport()->height()/2;
        QPointF viewportCenter(centerX, centerY);
        QPointF delta = mPanFirstPoint - mappedPoint;
        mPanFirstPoint = mappedPoint;
        centerOn(viewportCenter - delta);
    }
}

void SDrawingView::performReleaseEvent(QPoint p)
{
    QPointF mappedPoint = mapToScene(p);
    emit currentPointChanged(mappedPoint);
    if(eTool_Pen == mCurrentTool){
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

    emit clearCoefficients();
    clearInfos();

    // Generate a smooth path
    QPainterPath path = generatePath();

    // Add it as a single element to the scene
    QGraphicsPathItem* pathItem = mpScene->addPath(path, mPen);
    //pathItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);

    mItems << pathItem;

}

QPainterPath SDrawingView::generatePath()
{
    //return basicSmoothing();
    //return lagrangeSmoothing();
    //return cosineSmoothing();
    //return cubicSmoothing();
    return hermiteSmoothing();
}

QPainterPath SDrawingView::lagrangeSmoothing()
{
    QPainterPath path;

    // TODO: Implement me!

    return path;
}

QPainterPath SDrawingView::cosineSmoothing()
{
    QPainterPath path;

    int nbPoints = 20;

    // -- First Point ---------------------
    // At least 2 points for a line!
    if(!mPoints.empty()){
        // Set the origin of the path
        path.moveTo(mPoints.at(0));
    }

    // -- Intermediate Points -------------
    for(int i=1; i<mPoints.size()-1; i++){
        float xOrigin = mPoints.at(i-1).x();
        float yOrigin = mPoints.at(i-1).y();
        float xDest = mPoints.at(i).x();
        float yDest = mPoints.at(i).y();

        for(int j=nbPoints; j>0; j--){
            double mu = 1/(double)j;
            float xPoint = xOrigin + ((float)(xDest - xOrigin))/j;
            float yPoint = (float)cosineInterpolate((double)yOrigin, (double)yDest, mu);
            QPointF point(xPoint, yPoint);
            path.lineTo(point);

            emit addCoefficients(QPointF(xOrigin, yOrigin), point, QPointF(0,0), QPointF(0,0));
            addSplineInfos(QPointF(xOrigin, yOrigin), point, QPointF(0,0), QPointF(0,0));
        }
    }


    return path;
}

double SDrawingView::cosineInterpolate(double y1, double y2, double mu)
{
   double mu2;

   mu2 = (1-cos(mu*3.14))/2;
   return(y1*(1-mu2)+y2*mu2);
}

QPainterPath SDrawingView::cubicSmoothing()
{
    QPainterPath path;

    int nbPoints = 4;

    // -- First Point ---------------------
    // At least 2 points for a line!
    if(!mPoints.empty()){
        // Set the origin of the path
        path.moveTo(mPoints.at(0));
    }

    // -- Intermediate Points -------------
    for(int i=2; i<mPoints.size()-1; i++){
        QPointF p0 = mPoints.at(i-2);
        QPointF p1 = mPoints.at(i-1);
        QPointF p2 = mPoints.at(i);
        QPointF p3 = mPoints.at(i+1);

        float x1 = p1.x();
        double y1 = p1.y();
        float x2 = p2.x();
        double y0 = p0.y();
        double y2 = p2.y();
        double y3 = p3.y();

        for(int j=nbPoints; j>0; j--){
            double mu = 1/(double)j;
            float xPoint = x1 + ((float)(x2 - x1))/j;
            float yPoint = (float)cubicInterpolate(y0, y1, y2, y3, mu);
            //float yPoint = (float)cosineInterpolate((double)yOrigin, (double)yDest, mu);
            QPointF point(xPoint, yPoint);
            path.lineTo(point);

            emit addCoefficients(QPointF(x1, y1), point, QPointF(0,0), QPointF(0,0));
            addSplineInfos(QPointF(x1, y1), point, QPointF(0,0), QPointF(0,0));
        }
    }

    return path;
}

double SDrawingView::cubicInterpolate(double y0, double y1, double y2, double y3, double mu)
{
    double a0,a1,a2,a3,mu2;

    mu2 = mu*mu;
//    a0 = y3 - y2 - y0 + y1;
//    a1 = y0 - y1 - a0;
//    a2 = y2 - y0;
//    a3 = y1;

    a0 = -0.5*y0 + 1.5*y1 - 1.5*y2 + 0.5*y3;
    a1 = y0 - 2.5*y1 + 2*y2 - 0.5*y3;
    a2 = -0.5*y0 + 0.5*y2;
    a3 = y1;

    return(a0*mu*mu2+a1*mu2+a2*mu+a3);
}

QPainterPath SDrawingView::hermiteSmoothing()
{
    QPainterPath path;

    int nbPoints = 4;
    double tension = 0;
    double bias = 0.2;

    // -- First Point ---------------------
    // At least 2 points for a line!
    if(!mPoints.empty()){
        // Set the origin of the path
        path.moveTo(mPoints.at(0));
    }

    // -- Intermediate Points -------------
    for(int i=2; i<mPoints.size()-1; i++){
        QPointF p0 = mPoints.at(i-2);
        QPointF p1 = mPoints.at(i-1);
        QPointF p2 = mPoints.at(i);
        QPointF p3 = mPoints.at(i+1);

        float x1 = p1.x();
        double y1 = p1.y();
        float x2 = p2.x();
        double y0 = p0.y();
        double y2 = p2.y();
        double y3 = p3.y();

        for(int j=nbPoints; j>0; j--){
            double mu = 1/(double)j;
            float xPoint = x1 + ((float)(x2 - x1))/j;
            float yPoint = (float)hermiteInterpolate(y0, y1, y2, y3, mu, tension, bias);

            //float yPoint = (float)cubicInterpolate(y0, y1, y2, y3, mu);
            QPointF point(xPoint, yPoint);
            path.lineTo(point);

            emit addCoefficients(QPointF(x1, y1), point, QPointF(0,0), QPointF(0,0));
            addSplineInfos(QPointF(x1, y1), point, QPointF(0,0), QPointF(0,0));
        }
    }

    return path;
}

double SDrawingView::hermiteInterpolate(double y0, double y1, double y2, double y3, double mu, double tension, double bias)
{
    double m0,m1,mu2,mu3;
    double a0,a1,a2,a3;

    mu2 = mu * mu;
    mu3 = mu2 * mu;
    m0  = (y1-y0)*(1+bias)*(1-tension)/2;
    m0 += (y2-y1)*(1-bias)*(1-tension)/2;
    m1  = (y2-y1)*(1+bias)*(1-tension)/2;
    m1 += (y3-y2)*(1-bias)*(1-tension)/2;
    a0 =  2*mu3 - 3*mu2 + 1;
    a1 =    mu3 - 2*mu2 + mu;
    a2 =    mu3 -   mu2;
    a3 = -2*mu3 + 3*mu2;

    return(a0*y1+a1*m0+a2*m1+a3*y2);
}

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
        addSplineInfos(origin, endpoint, c1, c2);
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
    Q_UNUSED(c0);
    Q_UNUSED(c1);
    foreach(sSplineElement spline, mSplines){
        bool bShow = false;
        if(spline.p0 == p0 && spline.p1 == p1){
            bShow = true;
        }
        spline.pC0->setVisible(bShow);
        spline.pC0Label->setVisible(bShow);
        spline.pC1->setVisible(bShow);
        spline.pC1Label->setVisible(bShow);
        spline.pEndPoint->setVisible(bShow);
        spline.pOrigin->setVisible(bShow);
        spline.pP0Label->setVisible(bShow);
        spline.pP1Label->setVisible(bShow);
    }
}

void SDrawingView::clearInfos()
{
    foreach(sSplineElement spline, mSplines){
        mpScene->removeItem(spline.pC0);
        mpScene->removeItem(spline.pC0Label);
        mpScene->removeItem(spline.pC1);
        mpScene->removeItem(spline.pC1Label);
        mpScene->removeItem(spline.pEndPoint);
        mpScene->removeItem(spline.pOrigin);
        mpScene->removeItem(spline.pP0Label);
        mpScene->removeItem(spline.pP1Label);
    }
    mSplines.clear();
}

void SDrawingView::addSplineInfos(QPointF p0, QPointF p1, QPointF c0, QPointF c1)
{
    sSplineElement spline;
    QPen pointPen;
    pointPen.setColor(Qt::green);
    QPen coeffPen;
    coeffPen.setColor(Qt::red);
    int w = 2;

    spline.p0 = p0;
    spline.p1 = p1;
    spline.c0 = c0;
    spline.c1 = c1;
    spline.pOrigin = mpScene->addRect(p0.x()-w, p0.y()-w, 2*w, 2*w, pointPen);
    spline.pOrigin->setVisible(false);
    spline.pP0Label = mpScene->addSimpleText("P0");
    spline.pP0Label->setPos(p0.x() + w +1, p0.y() + w + 1);
    spline.pP0Label->setVisible(false);
    spline.pEndPoint = mpScene->addRect(p1.x()-w, p1.y()-w, 2*w, 2*w, pointPen);
    spline.pEndPoint->setVisible(false);
    spline.pP1Label = mpScene->addSimpleText("P1");
    spline.pP1Label->setPos(p1.x() + w +1, p1.y() + w + 1);
    spline.pP1Label->setVisible(false);
    spline.pC0 = mpScene->addEllipse(c0.x()-w, c0.y()-w, 2*w, 2*w, coeffPen);
    spline.pC0->setVisible(false);
    spline.pC0Label = mpScene->addSimpleText("C0");
    spline.pC0Label->setPos(c0.x() + w +1, c0.y() + w + 1);
    spline.pC0Label->setVisible(false);
    spline.pC1 = mpScene->addEllipse(c1.x()-w, c1.y()-w, 2*w, 2*w, coeffPen);
    spline.pC1->setVisible(false);
    spline.pC1Label = mpScene->addSimpleText("C1");
    spline.pC1Label->setPos(c1.x() + w +1, c1.y() + w + 1);
    spline.pC1Label->setVisible(false);

    mSplines << spline;
}

void SDrawingView::onLineWidthChanged(int w)
{
    mLineWidth = w;
    mPen.setWidth(mLineWidth);
}

void SDrawingView::onColorChanged(const QColor &color)
{
    mPen.setColor(color);
}

#include <QPainterPath>
#include <QApplication>
#include <QDesktopWidget>
#include <QTime>
#include <math.h>

#include "SGraphicsPathItem.h"
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
    mSmoothFactor = 75;
    mScaleFactor = 1.50;
    mPressure = 1.0;
    mZoomDepth = 0;
    mNextZValue = 0;
    mDrawingInProgress = false;

    mCurrentTool = eTool_Pen;
    mRed = 0;
    mGreen = 0;
    mBlue = 0;
    mAlpha = 255;
    mPen.setColor(QColor(mRed, mGreen, mBlue,mAlpha));
    mPen.setWidthF(3.0);
    mPen.setWidth(3);
    mPen.setCapStyle(Qt::RoundCap);
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
    mPressure = ev->pressure();
    mRotation = ev->rotation();
    mXTilt = ev->xTilt();
    mYTilt = ev->yTilt();
    if(QTabletEvent::TabletPress == ev->type()){
        performPressEvent(ev->pos());
    }else if(QTabletEvent::TabletMove == ev->type()){
        performMoveEvent(ev->pos());
    }else if(QTabletEvent::TabletRelease == ev->type()){
        performReleaseEvent(ev->pos());
    }
    mPressure = 1.0;
    mXTilt = 0;
    mYTilt = 0;
    mRotation = 0.0;
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

#ifdef ENABLE_DEBUG
        clearInfos();
#endif
        mPreviousPos.x = mappedPoint.x();
        mPreviousPos.y = mappedPoint.y();
        mPreviousPos.lineWidth = mPressure * (qreal)mLineWidth;
        mPreviousPos.rotation = mRotation;
        mPreviousPos.xTilt = mXTilt;
        mPreviousPos.ytilt = mYTilt;
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
            sPoint pt;
            pt.x = mappedPoint.x();
            pt.y = mappedPoint.y();
            pt.lineWidth = mPressure * (qreal)mLineWidth;
            pt.rotation = mRotation;
            pt.xTilt = mXTilt;
            pt.ytilt = mYTilt;

            draw(mPreviousPos, pt);
            mPreviousPos = pt;
            mPoints << pt;
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
        sPoint pt;
        pt.x = mappedPoint.x();
        pt.y = mappedPoint.y();
        pt.lineWidth = mPressure * (qreal)mLineWidth;
        pt.rotation = mRotation;
        pt.xTilt = mXTilt;
        pt.ytilt = mYTilt;

        draw(mPreviousPos, pt);
        mDrawingInProgress = false;
        mPoints << pt;

        // Refine the strokes
        optimizeLines();
    }
}

void SDrawingView::draw(sPoint prev, sPoint crnt)
{
    if(NULL != mpScene){
        qDebug() << "LineWidhtF: " << crnt.lineWidth;
        mPen.setWidthF(crnt.lineWidth);
        QRectF r;
        r.setX((qreal)prev.x);
        r.setY((qreal)prev.y);
        r.setWidth((qreal)(crnt.x - prev.x));
        r.setHeight((qreal)(crnt.y - prev.y));

        QGraphicsLineItem* pLine = new QGraphicsLineItem(prev.x, prev.y, crnt.x, crnt.y);
        pLine->setPen(mPen);
        pLine->setZValue(mNextZValue);
        mLines << pLine;
        mpScene->addItem(pLine);

        updateSceneRect(r);
    }
}

void SDrawingView::optimizeLines()
{
    // Remove the individual lines
    foreach(QGraphicsLineItem* line, mLines){
        mpScene->removeItem(line);
    }

#ifdef ENABLE_DEBUG
    emit clearCoefficients();
    clearInfos();
#endif

    // Generate a smooth path
    QPainterPath path = generatePath();

    // Add it as a single element to the scene
    SGraphicsPathItem* pathItem = new SGraphicsPathItem(path, mPen);
    mpScene->addItem(pathItem);
    pathItem->setZValue(mNextZValue);
    mNextZValue++;
    //pathItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);

    mItems << pathItem;

}

QPainterPath SDrawingView::generatePath()
{
    //return basicSmoothing();
    //return lagrangeSmoothing();
    //return cosineSmoothing();
    //return cubicSmoothing();
    return hermiteSmoothing(); // Take this one!
    //return noSmoothing();
}

QPainterPath SDrawingView::noSmoothing()
{
    QPainterPath path;

    if(!mPoints.empty()){
        // Set the origin of the path
        path.moveTo(mPoints.at(0).x, mPoints.at(0).y);
    }

    for(int i=1; i<mPoints.size(); i++)
    {
        path.lineTo(mPoints.at(i).x, mPoints.at(i).y);
    }

    return path;
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
        path.moveTo(mPoints.at(0).x, mPoints.at(0).y);
    }

    // -- Intermediate Points -------------
    for(int i=1; i<mPoints.size()-1; i++){
        float xOrigin = mPoints.at(i-1).x;
        float yOrigin = mPoints.at(i-1).y;
        float xDest = mPoints.at(i).x;
        float yDest = mPoints.at(i).y;

        for(int j=nbPoints; j>0; j--){
            double mu = 1/(double)j;
            float xPoint = xOrigin + ((float)(xDest - xOrigin))/j;
            float yPoint = (float)cosineInterpolate((double)yOrigin, (double)yDest, mu);
            QPointF point(xPoint, yPoint);
            path.lineTo(point);

#ifdef ENABLE_DEBUG
            emit addCoefficients(QPointF(xOrigin, yOrigin), point, QPointF(0,0), QPointF(0,0));
            addSplineInfos(QPointF(xOrigin, yOrigin), point, QPointF(0,0), QPointF(0,0));
#endif
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
        path.moveTo(mPoints.at(0).x, mPoints.at(0).y);
    }

    // -- Intermediate Points -------------
    for(int i=2; i<mPoints.size()-1; i++){
        QPointF p0 = QPointF(mPoints.at(i-2).x, mPoints.at(i-2).y);
        QPointF p1 = QPointF(mPoints.at(i-1).x, mPoints.at(i-1).y);
        QPointF p2 = QPointF(mPoints.at(i).x, mPoints.at(i).y);
        QPointF p3 = QPointF(mPoints.at(i+1).x, mPoints.at(i+1).y);

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

#ifdef ENABLE_DEBUG
            emit addCoefficients(QPointF(x1, y1), point, QPointF(0,0), QPointF(0,0));
            addSplineInfos(QPointF(x1, y1), point, QPointF(0,0), QPointF(0,0));
#endif
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
        path.moveTo(mPoints.at(0).x, mPoints.at(0).y); // [!] 99% of the smoothing time is used by this call
    }

    // -- Intermediate Points -------------
    for(int i=2; i<mPoints.size()-1; i++){
        QPointF p0 = QPointF(mPoints.at(i-2).x, mPoints.at(i-2).y);
        QPointF p1 = QPointF(mPoints.at(i-1).x, mPoints.at(i-1).y);
        QPointF p2 = QPointF(mPoints.at(i).x, mPoints.at(i).y);
        QPointF p3 = QPointF(mPoints.at(i+1).x, mPoints.at(i+1).y);

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
#ifdef ENABLE_DEBUG
            emit addCoefficients(QPointF(x1, y1), point, QPointF(0,0), QPointF(0,0));
            addSplineInfos(QPointF(x1, y1), point, QPointF(0,0), QPointF(0,0));
#endif
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
        path.moveTo(mPoints.at(0).x, mPoints.at(0).y);
    }

    // -- Intermediate Points -------------
    for(int i=1; i<mPoints.size()-1; i++){
        QPointF origin = QPointF(mPoints.at(i-1).x, mPoints.at(i-1).y);
        QPointF endpoint = QPointF(mPoints.at(i).x, mPoints.at(i).y);
        QPointF next = QPointF(mPoints.at(i+1).x, mPoints.at(i+1).y);
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

#ifdef ENABLE_DEBUG
        emit addCoefficients(origin, endpoint, c1, c2);
        addSplineInfos(origin, endpoint, c1, c2);
#endif
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

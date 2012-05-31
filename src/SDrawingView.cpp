#include <QPainterPath>
#include <QApplication>
#include <QDesktopWidget>
#include <QTime>
#include <QMimeData>
#include <QUrl>
#include <QPainterPath>
#include <QGraphicsPathItem>

#include <math.h>

#include "SGraphicsPathItem.h"
#include "SSelectionRect.h"
#include "SDrawingView.h"
#include "SCatmullRomSpline.h"
#include "SCubicSpline.h"

SDrawingView::SDrawingView(QWidget *parent, const char *name):QGraphicsView(parent)
  , mpScene(NULL)
  , mpRubber(NULL)
  , mpCurrentStroke(NULL)
{
    SETUP_STYLESHEET
    setAcceptDrops(true);
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
    mNextZValue = 1;
    mDrawingInProgress = false;

    mCurrentTool = eTool_Pen;
    mRed = 0;
    mGreen = 0;
    mBlue = 0;
    mAlpha = 255;
    mPen.setColor(QColor(mRed, mGreen, mBlue,mAlpha));
    mPen.setWidthF(3.0);
    mPen.setWidth(3);
    mLineWidth = 3;
    mPen.setCapStyle(Qt::RoundCap);
    mSelectionInProgress = false;
    mResizeInProgress = false;
}

SDrawingView::~SDrawingView()
{
    DELETEPTR(mpRubber);
    DELETEPTR(mpScene);
}

void SDrawingView::dragEnterEvent(QDragEnterEvent *ev)
{
    QMimeData* pMime = const_cast<QMimeData*>(ev->mimeData());
    if(NULL != pMime && (pMime->hasImage() || pMime->hasUrls())){
        ev->acceptProposedAction();
    }
}

void SDrawingView::dragMoveEvent(QDragMoveEvent *ev)
{
    ev->acceptProposedAction();
}

void SDrawingView::dragLeaveEvent(QDragLeaveEvent *ev)
{
    ev->accept();
}

void SDrawingView::dropEvent(QDropEvent *ev)
{
    QMimeData* pMime = const_cast<QMimeData*>(ev->mimeData());
    if(NULL != pMime){
        mpTmpPictureItem = new SGraphicsPictureItem/*QGraphicsPixmapItem*/();
        if(pMime->hasImage()){
            mpTmpPictureItem->setPixmap(QPixmap(pMime->imageData().toByteArray()));
        }else if(pMime->hasUrls()){
            foreach(QUrl url, pMime->urls()){
                mpTmpPictureItem->setPixmap(QPixmap(url.toLocalFile()));
            }
        }
        mItems << mpTmpPictureItem;
        mpScene->addItem(mpTmpPictureItem);
    }
    ev->acceptProposedAction();
}

void SDrawingView::keyPressEvent(QKeyEvent *ev)
{
    if(QKeyEvent::KeyPress == ev->type()){
        switch(ev->key()){
        case Qt::Key_Delete:
        case Qt::Key_Backspace:
            foreach(QGraphicsItem* i, mpScene->selectedItems()){
                if(NULL != i){
                    mpScene->removeItem(i);
                    DELETEPTR(i);
                }
            }
            mSelectedItems.clear();
            break;
        }
    }
}

void SDrawingView::mousePressEvent(QMouseEvent *ev)
{
    performPressEvent(ev->pos());
    ev->accept();
    QGraphicsView::mousePressEvent(ev);
}

void SDrawingView::mouseMoveEvent(QMouseEvent *ev)
{
    performMoveEvent(ev->pos());
    ev->accept();
    QGraphicsView::mouseMoveEvent(ev);
}

void SDrawingView::mouseReleaseEvent(QMouseEvent *ev)
{
    performReleaseEvent(ev->pos());
    ev->accept();
    QGraphicsView::mouseReleaseEvent(ev);
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
    ev->accept();
}

void SDrawingView::performPressEvent(QPoint p)
{
    QPointF mappedPoint = mapToScene(p);
    emit currentPointChanged(mappedPoint);
    if(eTool_Pen == mCurrentTool){
    	mpCurrentStroke = new SStrokeItem(mPen);
    	mpScene->addItem(mpCurrentStroke);
    	sPoint p;
    	p.x = mappedPoint.x();
    	p.y = mappedPoint.y();
    	p.lineWidth = mPressure * (qreal)mLineWidth;
    	p.rotation = mRotation;
    	p.xTilt = mXTilt;
    	p.ytilt = mYTilt;
    	mpCurrentStroke->addPoint(p);
    	mDrawingInProgress = true;
    }else if(eTool_Arrow == mCurrentTool){
        mSelectedCurrentPoint = p;
        // Select
        QGraphicsItem* pItem = itemAt(mappedPoint.x(), mappedPoint.y());
        SSelectionRect* pSelectRectItem = dynamic_cast<SSelectionRect*>(pItem);

        if(NULL != pSelectRectItem){
            pItem = pSelectRectItem->item();
            mResizeInProgress = pSelectRectItem->resizGripClicked();
        }

        if(!mSelectedItems.contains(pItem)){
            foreach(QGraphicsItem* item, mSelectedItems){
                if(NULL != item){
                    item->setSelected(false);
                }
            }
            mSelectedItems.clear();
            if(NULL != pItem){
                pItem->setSelected(true);
                mSelectedItems << pItem;
            }
            mSelectionInProgress = true;
        }else{
            // Unselect the other items in case of multiple selection
            foreach(QGraphicsItem* item, mSelectedItems){
                if(pItem != item){
                    item->setSelected(false);
                }
            }
            mSelectedItems.clear();
            mSelectedItems << pItem;
        }

        if(mSelectionInProgress){
            mSelectionOrigin = p;
            if(!mpRubber){
                mpRubber = new SRubberBand(this);
                mpRubber->setGeometry(QRect(mSelectionOrigin, QSize()));
                mpRubber->show();
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
    	if(mDrawingInProgress && NULL != mpCurrentStroke){
    		sPoint p;
			p.x = mappedPoint.x();
			p.y = mappedPoint.y();
			p.lineWidth = mPressure * (qreal)mLineWidth;
			p.rotation = mRotation;
			p.xTilt = mXTilt;
			p.ytilt = mYTilt;
			mpCurrentStroke->addPoint(p);
			drawCurrentLine();
    	}
    }else if(eTool_Arrow == mCurrentTool){
        if(mResizeInProgress){
            // The user is resizing the item
            //resizeItem(mSelectedItems.at(0), p);
        }else if(mSelectionInProgress && NULL != mpRubber){
            // The user is selecting with the rubber band
            mpRubber->setGeometry(QRect(mSelectionOrigin, p).normalized());
            int x = qMin(p.x(), mSelectionOrigin.x());
            int y = qMin(p.y(), mSelectionOrigin.y());
            QRect rubberRect(x, y, mpRubber->width(), mpRubber->height());
            QList<QGraphicsItem*> rubberItems = items(rubberRect);
            foreach (QGraphicsItem* it, mSelectedItems){
                if(!rubberItems.contains(it)){
                    it->setSelected(false);
                    mSelectedItems.remove(mSelectedItems.indexOf(it));
                }
            }
            foreach(QGraphicsItem* i, rubberItems){
                if(!mSelectedItems.contains(i)){
                    i->setSelected(true);
                    mSelectedItems << i;
                }
            }
        }else{
            // the user is doing a move of the item
            foreach(QGraphicsItem* item, mSelectedItems){
                if(NULL != item){
                    qreal dx = mSelectedCurrentPoint.x() - mappedPoint.x();
                    qreal dy = mSelectedCurrentPoint.y() - mappedPoint.y();
                    item->moveBy(-dx, -dy);
                }
            }
            mSelectedCurrentPoint = p;
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
    	if(mDrawingInProgress && NULL != mpCurrentStroke){
    		sPoint p;
			p.x = mappedPoint.x();
			p.y = mappedPoint.y();
			p.lineWidth = mPressure * (qreal)mLineWidth;
			p.rotation = mRotation;
			p.xTilt = mXTilt;
			p.ytilt = mYTilt;
			mpCurrentStroke->addPoint(p);
			mDrawingInProgress = false;
			drawCurrentLine();
    	}
    }else if(eTool_Arrow == mCurrentTool){
        if(mSelectionInProgress && NULL != mpRubber){
            mSelectionInProgress = false;
            mpRubber->hide();
            DELETEPTR(mpRubber);
        }
        mResizeInProgress = false;
    }
}

void SDrawingView::drawCurrentLine(){
	if(NULL != mpCurrentStroke){
		mpCurrentStroke->smooth();
	}
}

void SDrawingView::draw(sPoint prev, sPoint crnt)
{
	Q_UNUSED(prev);
    if(NULL != mpScene){
        mPen.setWidthF(crnt.lineWidth);
        QRectF r;

#ifdef REALTIME_INTERPOLATION
        foreach(QGraphicsItem* pItem, mLines){
            mpScene->removeItem(pItem);
        }
        mLines.clear();

        //QPainterPath p = catmullRomSmoothing();
        QPainterPath p = cubicSmoothing();

        QGraphicsPathItem* path = mpScene->addPath(p);
        mLines << path;

//        for(int i=0; i<p.elementCount(); i++){
//        	QPointF pt;
//        	pt.setX(p.elementAt(i).x);
//        	pt.setY(p.elementAt(i).y);
//        	QPen pen;
//        	pen.setColor(QColor(Qt::red));
//        	mpScene->addEllipse(pt.x()-2, pt.y()-2, 4, 4, pen);
//        }

        r = path->boundingRect();
        updateSceneRect(r);
#else
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
#endif
    }
}

void SDrawingView::optimizeLines()
{
    // Remove the individual lines
    foreach(QGraphicsItem* line, mLines){
        mpScene->removeItem(line);
    }

#ifdef ENABLE_DEBUG
    emit clearCoefficients();
    clearInfos();
#endif

    // Generate a smooth path
    QPainterPath path = generatePath();

    // Add it as a single element to the scene
    //SGraphicsPathItem* pathItem = new SGraphicsPathItem(path, mPen);
    SStrokeItem* pathItem = new SStrokeItem(mPen);
    pathItem->setPath(path);

    mpScene->addItem(pathItem);
    pathItem->setZValue(mNextZValue);

    mNextZValue++;

    mItems << pathItem;
}

QPainterPath SDrawingView::cubicPath(){
	QPainterPath path;



	return path;
}

QPainterPath SDrawingView::generatePath()
{
    //return hermiteSmoothing();
    //return catmullRomSmoothing();
	return cubicSmoothing();
}

QPainterPath SDrawingView::cubicSmoothing(){
	QPainterPath path;

	float distance = 0.0;
	float tolerance = 10.0;
	float precision = 10.0 * 0.01;

	// Smooth
	if(!mPoints.isEmpty()){
		// Set the origin of the path
		path.moveTo(mPoints.at(0).x, mPoints.at(0).y);

		QVector<QPointF> pts;
		foreach(sPoint pt, mPoints){
			pts << QPointF(pt.x, pt.y);
		}

#define TEST_SPLINE 1
#ifdef TEST_SPLINE
		pts.clear();
		pts << QPointF(100,200);
		pts << QPointF(200,150);
		pts << QPointF(230,200);
		pts << QPointF(350,100);
		pts << QPointF(400,400);
		pts << QPointF(480,380);
		path.moveTo(pts.at(0));
#endif

		SCubicSpline spline(pts, distance, tolerance, precision);
		QVector<QPointF> points = spline.getAllPoints();

		foreach(QPointF p, points){
			path.lineTo(p);
		}
	}

	return path;
}

QPainterPath SDrawingView::catmullRomSmoothing()
{
    QPainterPath path;
    int approxLength = 5;

#ifdef REALTIME_INTERPOLATION
    approxLength = 2;
#endif

    if(!mPoints.empty()){
        // Set the origin of the path
        path.moveTo(mPoints.at(0).x, mPoints.at(0).y);

        QVector<QPointF> pts;
        foreach(sPoint pt, mPoints){
            pts << QPointF(pt.x, pt.y);
        }

        SCatmullRomSpline spline(pts);
        QVector<QPointF> points = spline.getAllPoints(approxLength);

        foreach(QPointF p, points){
            path.lineTo(p);
        }
    }

    return path;
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

 // ----------------------------------------------------------------------------------------
SRubberBand::SRubberBand(QWidget *parent, const char *name):QRubberBand(QRubberBand::Rectangle, parent)
{
    setObjectName(name);
    mPenColor.setRed(0);
    mPenColor.setGreen(0);
    mPenColor.setBlue(255);
    mPenColor.setAlpha(255);

    mBrushColor.setRed(0);
    mBrushColor.setGreen(0);
    mBrushColor.setBlue(255);
    mBrushColor.setAlpha(200);
}

SRubberBand::~SRubberBand()
{

}

void SRubberBand::paintEvent(QPaintEvent* ev)
{
    Q_UNUSED(ev);
    QPainter p(this);
    p.setPen(QPen(mPenColor, 3));
    p.setBackground(QBrush(mBrushColor));
    p.drawRect(rect());
}

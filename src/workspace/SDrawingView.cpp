#include <QPainterPath>
#include <QApplication>
#include <QDesktopWidget>
#include <QTime>
#include <QMimeData>
#include <QUrl>
#include <QPainterPath>
#include <QGraphicsPathItem>
#include <QGraphicsRectItem>
#include <QGLWidget>

#include <math.h>

#include "graphicsitems/SGraphicsPathItem.h"
#include "workspace/SSelectionRect.h"
#include "SDrawingView.h"
#include "maths/SCatmullRomSpline.h"
#include "maths/SCubicSpline.h"
#include "tools/SToolsController.h"
#include "drawing/SDrawingController.h"

SDrawingView::SDrawingView(QWidget *parent, const char *name):QGraphicsView(parent)
  , mpScene(NULL)
  , mpRubber(NULL)
  , mpCurrentStroke(NULL)
  , mCurrentStroke(NULL)
  , mpDrawingController(NULL)
{
	// General
    SETUP_STYLESHEET
    setObjectName(name);
    mpDrawingController = SDrawingController::drawingController();

    setInteractive(false);

    // Flags
    setAcceptDrops(true);
    setWindowFlags(Qt::FramelessWindowHint);
    setFrameStyle (QFrame::NoFrame);
    setRenderHint(QPainter::Antialiasing, true);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setCacheMode(QGraphicsView::CacheBackground);

    // Scene
    mpScene = new SDrawingScene(this);
    setScene(mpScene);
    mItems.clear();

    // Drawing configuration
    mSmoothFactor = 75;
    mScaleFactor = 1.50;
    mPressure = 1.0;
    mZoomDepth = 0;
    mNextZValue = 1;
    mDrawingInProgress = false;
    mRed = 0;
    mGreen = 0;
    mBlue = 0;
    mAlpha = 255;
    mPen.setColor(QColor(mRed, mGreen, mBlue,mAlpha));
    mPen.setCapStyle(Qt::RoundCap);
    mLineWidth = mpDrawingController->currentBrush()->width();
    mPen.setWidth(mLineWidth);
    mPen.setCapStyle(Qt::RoundCap);
    mSelectionInProgress = false;
    mResizeInProgress = false;

    // Signal/Slots
    connect(mpDrawingController, SIGNAL(brushChanged(SBrush*)), this, SLOT(onBrushChanged(SBrush*)));
}

SDrawingView::~SDrawingView()
{
    //DELETEPTR(mpDrawingController);
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
        mpTmpPictureItem = new SGraphicsPictureItem();
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
	eTool tool = SToolsController::toolsController()->currentTool();
    QPointF mappedPoint = mapToScene(p);
    if(eTool_Pen == tool){
    	mCurrentStroke = new SStroke(mPen, mpScene);
        sPoint* pt = updatePenParameters(QPoint(mappedPoint.x(), mappedPoint.y()));
    	mCurrentStroke->addPoint(pt);
    	mDrawingInProgress = true;
    }else if(eTool_Arrow == tool){
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
    }else if(eTool_Pan == tool){
        // Pan
        mPanFirstPoint = mappedPoint;
    }
}

void SDrawingView::performMoveEvent(QPoint p)
{
	eTool tool = SToolsController::toolsController()->currentTool();
    QPointF mappedPoint = mapToScene(p);
    if(eTool_Pen == tool){
    	if(mDrawingInProgress && NULL != mCurrentStroke){
            sPoint* pt = updatePenParameters(QPoint(mappedPoint.x(), mappedPoint.y()));
			mCurrentStroke->addPoint(pt);
			drawCurrentLine();
    	}
    }else if(eTool_Arrow == tool){
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
    }else if(eTool_Pan == tool){
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
	eTool tool = SToolsController::toolsController()->currentTool();
    QPointF mappedPoint = mapToScene(p);
    if(eTool_Pen == tool){
    	if(mDrawingInProgress && NULL != mCurrentStroke){
            sPoint* pt = updatePenParameters(QPoint(mappedPoint.x(), mappedPoint.y()));
			mCurrentStroke->addPoint(pt);
			mDrawingInProgress = false;
			drawCurrentLine();
    	}
    }else if(eTool_Arrow == tool){
        if(mSelectionInProgress && NULL != mpRubber){
            mSelectionInProgress = false;
            mpRubber->hide();
            DELETEPTR(mpRubber);
        }
        mResizeInProgress = false;
    }
}

void SDrawingView::drawCurrentLine(){
	if(NULL != mCurrentStroke){
        mCurrentStroke->generatePath();
	}
}

void SDrawingView::onSmoothnessChanged(int smoothFactor)
{
    mSmoothFactor = smoothFactor;
}

void SDrawingView::clearPage()
{
    mPoints.clear();
    mLines.clear();
    mItems.clear();
    mpScene->clear();
}


void SDrawingView::onBrushChanged(SBrush* b){
	if(NULL != b){
		mLineWidth = b->width();
		mPen.setWidth(mLineWidth);
		mPen.setColor(b->color());
	}
}

void SDrawingView::performDrawingJob(eInputType input){
	QPoint p(mpDrawingController->point.x() - pos().x(), mpDrawingController->point.y() - pos().y());
	switch(input){
	case eInputType_MousePress:
	case eInputType_TabletPress:
		performPressEvent(p);
		break;
	case eInputType_MouseMove:
	case eInputType_TabletMove:
		performMoveEvent(p);
		break;
	case eInputType_MouseRelease:
	case eInputType_TabletRelease:
		performReleaseEvent(p);
		break;
	}
}

sPoint* SDrawingView::updatePenParameters(QPoint point){
    sPoint* p = new sPoint();
    p->x = point.x();
    p->y = point.y();
    p->pressure = mpDrawingController->pressure;
    p->rotation = mpDrawingController->rotation;
    p->xTilt = mpDrawingController->xTilt;
    p->ytilt = mpDrawingController->yTilt;

	return p;
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

#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsItem>

#include "SLeafWidget.h"
#include "document/SDocumentManager.h"
#include "tools/SToolsController.h"

SLeafWidget::SLeafWidget(QWidget* parent, const char* name):QGraphicsView(parent)
	, mpScene(NULL)
	, mpDrawingController(NULL)
{
	setObjectName(name);
    setStyleSheet("background:transparent;");
    mpDrawingController = SDrawingController::drawingController();

    mpScene = new QGraphicsScene();
    QRect r = QApplication::desktop()->rect();
    mpScene->setSceneRect(0, 0, r.width()*10, r.height()*10);
    setScene(mpScene);

    mScaleFactor = 1.25;
    mZoomDepth = MAX_ZOOM_LEVEL/2;
}

SLeafWidget::~SLeafWidget(){
	DELETEPTR(mpDrawingController);
    DELETEPTR(mpScene);
}

void SLeafWidget::tabletEvent(QTabletEvent* ev){
	mpDrawingController->pressure = ev->pressure();
	mpDrawingController->rotation = ev->rotation();
	mpDrawingController->tangentialPressure = ev->tangentialPressure();
	mpDrawingController->xTilt = ev->xTilt();
	mpDrawingController->yTilt = ev->yTilt();
	mpDrawingController->point = mapToScene(ev->pos());

	if(QTabletEvent::TabletPress == ev->type()){
		if(performPressEvent(mpDrawingController->point)){
			SDocumentManager::documentManager()->forwardEventToLayer(eInputType_TabletPress);
		}
	}else if(QTabletEvent::TabletMove == ev->type()){
		if(performMoveEvent(mpDrawingController->point)){
			SDocumentManager::documentManager()->forwardEventToLayer(eInputType_TabletMove);
		}
	}else if(QTabletEvent::TabletRelease == ev->type()){
		if(performReleaseEvent(mpDrawingController->point)){
			SDocumentManager::documentManager()->forwardEventToLayer(eInputType_TabletRelease);
		}
	}
}

void SLeafWidget::mousePressEvent(QMouseEvent* ev){
	setMouseEventValues(ev);
	if(performPressEvent(mpDrawingController->point)){
		SDocumentManager::documentManager()->forwardEventToLayer(eInputType_MousePress);
	}
}

void SLeafWidget::mouseMoveEvent(QMouseEvent* ev){
	setMouseEventValues(ev);
	if(performMoveEvent(mpDrawingController->point)){
		SDocumentManager::documentManager()->forwardEventToLayer(eInputType_MouseMove);
	}
}

void SLeafWidget::mouseReleaseEvent(QMouseEvent* ev){
	setMouseEventValues(ev);
	if(performReleaseEvent(mpDrawingController->point)){
		SDocumentManager::documentManager()->forwardEventToLayer(eInputType_MouseRelease);
	}
}

void SLeafWidget::setMouseEventValues(QMouseEvent* ev){
	mpDrawingController->pressure = 1.0;
	mpDrawingController->rotation = 0.0;
	mpDrawingController->tangentialPressure = 1.0;
	mpDrawingController->xTilt = 0;
	mpDrawingController->yTilt = 0;
	mpDrawingController->point = mapToScene(ev->pos());
}

void SLeafWidget::wheelEvent(QWheelEvent* ev){
	if(ev->delta() > 0 && mZoomDepth < MAX_ZOOM_LEVEL){
		zoomIn(mapToScene(ev->pos()));
		mZoomDepth++;
	}else if(mZoomDepth > 0){
		zoomOut(mapToScene(ev->pos()));
		mZoomDepth--;
	}
}

void SLeafWidget::zoomIn(QPointF p){
	scale(mScaleFactor, mScaleFactor);
	centerOn(p);
}

void SLeafWidget::zoomOut(QPointF p){
	scale(1.0/mScaleFactor, 1.0/mScaleFactor);
	centerOn(p);
}

bool SLeafWidget::performPressEvent(QPointF p){
	bool forward = true;
	eTool tool = SToolsController::toolsController()->currentTool();
	if(eTool_ZoomIn == tool){
		zoomIn(p);
		forward = false;
	}else if(eTool_ZoomOut == tool){
		zoomOut(p);
		forward = false;
	}else if(eTool_Pan == tool){
		mLastPanPoint = p;
		forward = false;
	}
	return forward;
}

bool SLeafWidget::performMoveEvent(QPointF p){
	bool forward = true;
	eTool tool = SToolsController::toolsController()->currentTool();
	if(eTool_Pan == tool){
		QPointF delta = mLastPanPoint - p;
		mLastPanPoint = p;

		//setCenter();

		forward = false;
	}
	return forward;
}

bool SLeafWidget::performReleaseEvent(QPointF p){
	Q_UNUSED(p);
	bool forward = true;
	eTool tool = SToolsController::toolsController()->currentTool();
	if(eTool_Pan == tool){
		mLastPanPoint = QPointF();
		forward = false;
	}
	return forward;
}

void SLeafWidget::setCenter(const QPointF& centerPoint) {
    //Get the rectangle of the visible area in scene coords
    QRectF visibleArea = mapToScene(rect()).boundingRect();

    //Get the scene area
    QRectF sceneBounds = sceneRect();

    double boundX = visibleArea.width() / 2.0;
    double boundY = visibleArea.height() / 2.0;
    double boundWidth = sceneBounds.width() - 2.0 * boundX;
    double boundHeight = sceneBounds.height() - 2.0 * boundY;

    //The max boundary that the centerPoint can be to
    QRectF bounds(boundX, boundY, boundWidth, boundHeight);

    if(bounds.contains(centerPoint)) {
        //We are within the bounds
    	mCurrentCenterPoint = centerPoint;
    } else {
        //We need to clamp or use the center of the screen
        if(visibleArea.contains(sceneBounds)) {
            //Use the center of scene ie. we can see the whole scene
        	mCurrentCenterPoint = sceneBounds.center();
        } else {

        	mCurrentCenterPoint = centerPoint;

            //We need to clamp the center. The centerPoint is too large
            if(centerPoint.x() > bounds.x() + bounds.width()) {
            	mCurrentCenterPoint.setX(bounds.x() + bounds.width());
            } else if(centerPoint.x() < bounds.x()) {
            	mCurrentCenterPoint.setX(bounds.x());
            }

            if(centerPoint.y() > bounds.y() + bounds.height()) {
            	mCurrentCenterPoint.setY(bounds.y() + bounds.height());
            } else if(centerPoint.y() < bounds.y()) {
            	mCurrentCenterPoint.setY(bounds.y());
            }

        }
    }

    //Update the scrollbars
    centerOn(mCurrentCenterPoint);
}

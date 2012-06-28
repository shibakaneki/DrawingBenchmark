#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsItem>

#include "SLeafWidget.h"
#include "document/SDocumentManager.h"

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
	qDebug() << mpDrawingController->point;
	if(QTabletEvent::TabletPress == ev->type()){
		SDocumentManager::documentManager()->forwardEventToLayer(eInputType_TabletPress);
	}else if(QTabletEvent::TabletMove == ev->type()){
		SDocumentManager::documentManager()->forwardEventToLayer(eInputType_TabletMove);
	}else if(QTabletEvent::TabletRelease == ev->type()){
		SDocumentManager::documentManager()->forwardEventToLayer(eInputType_TabletRelease);
	}
}

void SLeafWidget::mousePressEvent(QMouseEvent* ev){
	setMouseEventValues(ev);
	SDocumentManager::documentManager()->forwardEventToLayer(eInputType_MousePress);
}

void SLeafWidget::mouseMoveEvent(QMouseEvent* ev){
	setMouseEventValues(ev);
	SDocumentManager::documentManager()->forwardEventToLayer(eInputType_MouseMove);
}

void SLeafWidget::mouseReleaseEvent(QMouseEvent* ev){
	setMouseEventValues(ev);
	SDocumentManager::documentManager()->forwardEventToLayer(eInputType_MouseRelease);
}

void SLeafWidget::setMouseEventValues(QMouseEvent* ev){
	mpDrawingController->pressure = 1.0;
	mpDrawingController->rotation = 0.0;
	mpDrawingController->tangentialPressure = 1.0;
	mpDrawingController->xTilt = 0;
	mpDrawingController->yTilt = 0;
	mpDrawingController->point = mapToScene(ev->pos());
	qDebug() << mpDrawingController->point;
}

#include "drawing/SDrawingController.h"
#include "drawing/SDefaultBrush.h"

SDrawingController* SDrawingController::mpInstance = NULL;

SDrawingController::SDrawingController(){
	setCurrentBrush(new SBrush());
}

SDrawingController::~SDrawingController(){
	DELETEPTR(mpBrush);
}

SDrawingController* SDrawingController::drawingController(){
	if(NULL == mpInstance){
		mpInstance = new SDrawingController();
	}
	return mpInstance;
}

void SDrawingController::onWidthChanged(int w){
	if(NULL != mpBrush){
		mpBrush->setWidth(w);
		emit brushChanged(mpBrush);
	}
}

void SDrawingController::onColorChanged(QColor c){
	if(NULL != mpBrush){
		mpBrush->setColor(c);
		emit brushChanged(mpBrush);
	}
}

SBrush* SDrawingController::currentBrush(){
	return mpBrush;
}

void SDrawingController::setCurrentBrush(SBrush* b){
	if(NULL != b){
		mpBrush = b;
		emit brushChanged(mpBrush);
	}
}

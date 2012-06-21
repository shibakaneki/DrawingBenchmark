#include "drawing/SDrawingController.h"
#include "drawing/SDefaultBrush.h"

SDrawingController* SDrawingController::mpInstance = NULL;

SDrawingController::SDrawingController():mpBrush(NULL)
	, mInterpolStep(5)
	, mInterpolLevel(5)
{
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

void SDrawingController::setWidthPressureSensitive(bool sensitive){
	if(NULL != mpBrush){
		mpBrush->setWidthPressureSensitive(sensitive);
		emit brushChanged(mpBrush);
	}
}

void SDrawingController::setOpacityPressureSensitive(bool sensitive){
	if(NULL != mpBrush){
			mpBrush->setOpacityPressureSensitive(sensitive);
			emit brushChanged(mpBrush);
		}
}

int SDrawingController::interpolationStep(){
	return mInterpolStep;
}

int SDrawingController::interpolationLevel(){
	return mInterpolLevel;
}

void SDrawingController::setInterpolationLevel(int l){
	mInterpolLevel = l;
}

void SDrawingController::setInterpolationStep(int s){
	mInterpolStep = s;
}

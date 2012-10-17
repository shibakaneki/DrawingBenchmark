#include "drawing/SDrawingController.h"
#include "drawing/SDefaultBrush.h"

SDrawingController* SDrawingController::mpInstance = NULL;

SDrawingController::SDrawingController():mpBrush(NULL)
    , mSpacing(1)
    , mCurrentColor(eColorRole_Foreground)
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

void SDrawingController::setWidth(int w){
	if(NULL != mpBrush){
		mpBrush->setWidth(w);
	}
}

int SDrawingController::width(){
    if(NULL != mpBrush){
        return mpBrush->width();
    }
    return 1;
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
	}
}

void SDrawingController::setOpacityPressureSensitive(bool sensitive){
	if(NULL != mpBrush){
			mpBrush->setOpacityPressureSensitive(sensitive);
		}
}

void SDrawingController::setSpacing(int s){
    if(NULL != mpBrush){
        mpBrush->setSpacing(s);
    }
}

int SDrawingController::spacing(){
    if(NULL != mpBrush){
        return mpBrush->spacing();
    }
    return 1;
}

void SDrawingController::setHardness(int h){
    if(NULL != mpBrush){
        mpBrush->setHardness(h);
    }
}

int SDrawingController::hardness(){
    if(NULL != mpBrush){
        return mpBrush->hardness();
    }
    return 1;
}

QColor SDrawingController::foregroundColor(){
    return mForegroundColor;
}

void SDrawingController::setForegroundColor(QColor c){
    mForegroundColor = c;
    if(eColorRole_Foreground == mCurrentColor &&NULL !=  mpBrush){
        mpBrush->setColor(mForegroundColor);
    }
}

QColor SDrawingController::backgroundColor(){
    return mBackgroundColor;
}

void SDrawingController::setBackgroundColor(QColor c){
    mBackgroundColor = c;
    if(eColorRole_Background == mCurrentColor &&NULL !=  mpBrush){
        mpBrush->setColor(mBackgroundColor);
    }
}

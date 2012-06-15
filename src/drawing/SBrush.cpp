#include "SBrush.h"

SBrush::SBrush(){
	mWidth = 3;
	mColor = QColor(Qt::black);
	mName = "";
	mWidthPressure = false;
	mOpacityPressure = false;
}

SBrush::~SBrush(){

}

int SBrush::width(){
	return mWidth;
}

void SBrush::setWidth(int w){
	mWidth = w;
}

QColor SBrush::color(){
	return mColor;
}

void SBrush::setColor(QColor c){
	mColor = c;
}

QString SBrush::name(){
	return mName;
}

void SBrush::rename(const QString& n){
	mName = n;
}

bool SBrush::isWidthPressureSensitive(){
	return mWidthPressure;
}

void SBrush::setWidthPressureSensitive(bool s){
	mWidthPressure = s;
}

bool SBrush::isOpacityPressureSensitive(){
	return mOpacityPressure;
}

void SBrush::setOpacityPressureSensitive(bool s){
	mOpacityPressure = s;
}

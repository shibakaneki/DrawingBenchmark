#include "SBrush.h"

SBrush::SBrush(){
	mWidth = 3;
	mColor = QColor(Qt::black);
	mName = "";
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

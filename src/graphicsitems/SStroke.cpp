#include "SStroke.h"
#include "maths/SGeometryHelper.h"
#include "drawing/SDrawingController.h"

//#define SMOOTH	1

SStroke::SStroke(QPen p, QGraphicsScene* s){
	mPen = p;
	mpScene = s;
}

SStroke::~SStroke(){

}

void SStroke::addPoint(sPoint p){
	mPoints << p;
}

QGraphicsItem* SStroke::smooth(){
#ifdef SMOOTH

#else
	sLine line;
	SBrush* pBrush = SDrawingController::drawingController()->currentBrush();
	if(mPoints.size() <= 1){
		line.p1 = mPoints.at(0);
		line.p2 = line.p1;
	}else{
		line.p1 = mPoints.at(mPoints.size()-2);
		line.p2 = mPoints.at(mPoints.size()-1);
	}
	mpSegment = new SGraphicsPolygonItem(SGeometryHelper::lineToPolygon(line, (qreal)pBrush->width(), pBrush->isWidthPressureSensitive()));
	if(pBrush->isOpacityPressureSensitive()){
		QColor col = mPen.color();
		qreal alphaCol = col.alpha();
		col.setAlpha(line.p2.pressure*alphaCol);
		mpSegment->setColor(col);
	}else{
		mpSegment->setColor(mPen.color());
	}

	if(!mpSegment->brush().isOpaque()){
		for(int i=0; i<mSegments.size(); i++){
			SGraphicsPolygonItem* prevPoly = mSegments.at(i);
			mpSegment->subtract(prevPoly);
		}
	}
	mSegments << mpSegment;
	mpScene->addItem(mpSegment);
	return mpSegment;
#endif
}


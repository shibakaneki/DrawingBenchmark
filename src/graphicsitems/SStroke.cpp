#include <math.h>

#include "SStroke.h"
#include "maths/SGeometryHelper.h"

SStroke::SStroke(QPen p, QGraphicsScene* s){
	mPen = p;
	mpScene = s;
	mpDrawingController = SDrawingController::drawingController();
}

SStroke::~SStroke(){

}

void SStroke::addPoint(sPoint p){
	mPoints << p;
}

QGraphicsItem* SStroke::generateLastSegment(){
	QList<sPoint>* points;

#ifdef SMOOTH
	points = &mInterpolPoints;
#else
	points = &mPoints;
#endif

	if(!points->empty()){
		sLine line;
		SBrush* pBrush = mpDrawingController->currentBrush();
		if(points->size() <= 1){
			line.p1 = points->at(0);
			line.p2 = line.p1;
		}else{
			line.p1 = points->at(points->size()-2);
			line.p2 = points->at(points->size()-1);
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
	}
	return NULL;
}

QList<sCoord> SStroke::generateXPolynomials(){
	QList<sCoord> pts;
	int start = mPoints.size() - 1 - mpDrawingController->interpolationLevel();
	if(start < 0){
		start = 0;
	}
	for(int i=start; i<mPoints.size(); i++){
		sPoint pt = mPoints.at(i);
		sCoord c;
		c.coord = pt.x;
		c.pressure = pt.pressure;
		c.tilt = pt.xTilt;
		c.rotation = pt.rotation;
		pts << c;
	}

	return generatePolynomials(pts);
}

QList<sCoord> SStroke::generateYPolynomials(){
	QList<sCoord> pts;
	int start = mPoints.size() - 1 - mpDrawingController->interpolationLevel();
	if(start < 0){
		start = 0;
	}
	for(int i=start; i<mPoints.size(); i++){
		sPoint pt = mPoints.at(i);
		sCoord c;
		c.coord = pt.y;
		c.pressure = pt.pressure;
		c.tilt = pt.ytilt;
		c.rotation = pt.rotation;
		pts << c;
	}

	return generatePolynomials(pts);
}

QList<sCoord> SStroke::generatePolynomials(QList<sCoord> coords){
	QList<sCoord> pts;

	int n = coords.size() - 1;

	float gamma[n+1];
	float delta[n+1];
	float D[n+1];
	int i;

	gamma[0] = 1.0f/2.0f;
	for(i=1; i<n; i++){
		gamma[i] = 1/(4-gamma[i-1]);
	}
	gamma[n] = 1/(2-gamma[n-1]);

	delta[0] = 3*(coords.at(1).coord-coords.at(0).coord)*gamma[0];
	for(i=1; i<n; i++){
		delta[i] = (3*(coords.at(i+1).coord-coords.at(i-1).coord)-delta[i-1])*gamma[i];
	}
	delta[n] = (3*(coords.at(n).coord-coords.at(n-1).coord)-delta[n-1])*gamma[n];

	D[n] = delta[n];
	for(i=n-1; i>=0; i--){
		D[i] = delta[i] - gamma[i]*D[i+1];
	}

	// now compute the coefficients of the cubics
	for(i=0; i < n; i++){
		sCoord c = coords.at(i);
		c.polynomial = SCubicPolynomial((float)coords.at(i).coord, D[i], 3*(coords.at(i+1).coord - coords.at(i).coord) - 2*D[i] - D[i+1], 2*(coords.at(i).coord - coords.at(i+1).coord) + D[i] + D[i+1]);
		pts << c;
	}

	return pts;
}

void SStroke::interpolateSegments(){
	mInterpolPoints.clear();
	foreach(SGraphicsPolygonItem* poly, mTempSegments){
		mSegments.removeOne(poly);
		mpScene->removeItem(poly);
	}
	mTempSegments.clear();

	int step = mpDrawingController->interpolationStep();
	QList<sCoord> xPol = generateXPolynomials();
	QList<sCoord> yPol = generateYPolynomials();

	sCoord scX = xPol.at(0);
	sCoord scY = yPol.at(0);
	SCubicPolynomial scpX = scX.polynomial;
	SCubicPolynomial scpY = scY.polynomial;
	sPoint origin;
	sPoint dest;
	origin.x = roundf(scpX.eval(0));
	origin.y = roundf(scpX.eval(0));
	origin.pressure = scX.pressure;
	origin.rotation = scX.rotation;
	origin.xTilt = scX.tilt;
	origin.ytilt = scY.tilt;

	for(int i=0; i<xPol.size(); i++) {
		if(!mSegments.empty() && 0 == i){
			mInterpolPoints << mLastStoredCoord;
			qgraphicsitem_cast<SGraphicsPolygonItem*>(generateLastSegment());
			origin = mLastStoredCoord;
		}
		for(int j=1; j<=step; j++){
		  float u = j / (float) step;
		  sCoord scXU = xPol.at(i);
		  SCubicPolynomial scpXU = scXU.polynomial;
		  sCoord scYU = yPol.at(i);
		  SCubicPolynomial scpYU = scYU.polynomial;
		  dest.x = roundf(scpXU.eval(u));
		  dest.y = roundf(scpYU.eval(u));
		  dest.pressure = scXU.pressure;
		  dest.rotation = scXU.rotation;
		  dest.xTilt = scXU.tilt;
		  dest.ytilt = scYU.tilt;

		  mInterpolPoints << dest;
		  origin = dest;

		  SGraphicsPolygonItem* item = qgraphicsitem_cast<SGraphicsPolygonItem*>(generateLastSegment());
		  if(NULL != item && i > 0){
			  mTempSegments << item;
		  }
		}
		if(0 == i){
			mLastStoredCoord = dest;
		}
	}
}

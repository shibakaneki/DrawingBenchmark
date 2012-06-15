/*
 * SStrokeItem.cpp
 *
 *  Created on: May 30, 2012
 *      Author: kindov
 */

#include <math.h>
#include "SStrokeItem.h"
#include "maths/SGeometryHelper.h"

//#define SMOOTH	1
//#define USE_UBPOLY	1

SStrokeItem::SStrokeItem(const QPen& pen, QGraphicsItem* parent):QGraphicsPathItem(parent){
	setPen(pen);
	setPath(mPath);
}

SStrokeItem::~SStrokeItem(){

}

void SStrokeItem::addPoint(sPoint p){
	mPoints << p;
}

void SStrokeItem::smooth(){
	QPainterPath path;

#ifdef SMOOTH
	QVector<sPolyPoint> xPol = generateXPolynomials();
	QVector<sPolyPoint> yPol = generateYPolynomials();

#ifdef USE_UBPOLY
	sPolyPoint sppX = xPol.at(0);
	SCubicPolynomial scpX = sppX.poly;
	sPolyPoint sppY = yPol.at(0);
	SCubicPolynomial scpY = sppY.poly;
	sPoint origin;
	origin.x = roundf(scpX.eval(0));
	origin.y = roundf(scpY.eval(0));
	origin.lineWidth = sppX.width;
	path.moveTo(origin.x, origin.y);

	for(int i=0; i<xPol.size(); i++) {
		QVector<sPoint> xPts;
		QVector<sPoint> yPts;
	  for(int j=1; j<=INTERPOL_STEP; j++) {
		  float u = j / (float) INTERPOL_STEP;
		  sPolyPoint sppXU = xPol.at(i);
		  SCubicPolynomial scpXU = sppXU.poly;
		  sPolyPoint sppYU = yPol.at(i);
		  SCubicPolynomial scpYU = sppYU.poly;
		  sPoint dest;
		  dest.x = roundf(scpXU.eval(u));
		  dest.y = roundf(scpYU.eval(u));
		  dest.lineWidth = sppXU.width;

		  sLine li;
		  li.p1 = origin;
		  li.p2 = dest;

		  path.addPolygon(SGeometryHelper::lineToPolygon(li));
		  origin = dest;
	  }
	}
#else
	QPolygonF p;

	sPolyPoint sppX = xPol.at(0);
	SCubicPolynomial scpX = sppX.poly;
	sPolyPoint sppY = yPol.at(0);
	SCubicPolynomial scpY = sppY.poly;
	p.append(QPointF(roundf(scpX.eval(0)), roundf(scpY.eval(0))));
	for(int i=0; i<xPol.size(); i++) {
	  for(int j=1; j<=INTERPOL_STEP; j++) {
		  float u = j / (float) INTERPOL_STEP;
		  sPolyPoint sppXU = xPol.at(i);
		  SCubicPolynomial scpXU = sppXU.poly;
		  sPolyPoint sppYU = yPol.at(i);
		  SCubicPolynomial scpYU = sppYU.poly;
		  p.append(QPointF(round(scpXU.eval(u)), round(scpYU.eval(u))));
	  }
	}
	path.addPolygon(p);
#endif

#else
	path.moveTo(QPointF(mPoints.at(0).x, mPoints.at(0).y));
	for(int i=1; i<mPoints.size(); i++){
		path.lineTo(QPointF(mPoints.at(i).x, mPoints.at(i).y));
	}

#endif
	setPath(path);
}

QVector<sPolyPoint> SStrokeItem::generateXPolynomials(){
	QVector<sCoordWidth> xCoords;
	foreach(sPoint pt, mPoints){
		sCoordWidth cw;
		cw.coord = pt.x;
		//cw.width = pt.lineWidth;
		xCoords << cw;
	}
	return generatePolynomials(xCoords);
}

QVector<sPolyPoint> SStrokeItem::generateYPolynomials(){
	QVector<sCoordWidth> yCoords;
	foreach(sPoint pt, mPoints){
		sCoordWidth cw;
		cw.coord = pt.y;
		//cw.width = pt.lineWidth;
		yCoords << cw;
	}
	return generatePolynomials(yCoords);
}

QVector<sPolyPoint> SStrokeItem::generatePolynomials(QVector<sCoordWidth> coords){
	QVector<sPolyPoint> polys;
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
		sPolyPoint pt;
		pt.poly = SCubicPolynomial((float)coords.at(i).coord, D[i], 3*(coords.at(i+1).coord - coords.at(i).coord) - 2*D[i] - D[i+1], 2*(coords.at(i).coord - coords.at(i+1).coord) + D[i] + D[i+1]);
		pt.width = coords.at(i).width;
		polys << pt;
	}

	return polys;
}


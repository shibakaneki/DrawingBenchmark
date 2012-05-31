/*
 * SStrokeItem.cpp
 *
 *  Created on: May 30, 2012
 *      Author: kindov
 */

#include <math.h>
#include "SStrokeItem.h"

#define SMOOTH	1

SStrokeItem::SStrokeItem(const QPen& pen, QGraphicsItem* parent):QGraphicsPathItem(parent){

}

SStrokeItem::~SStrokeItem(){

}

void SStrokeItem::addPoint(sPoint p){
	mPoints << p;
}

void SStrokeItem::smooth(){
	QPainterPath path;

#ifdef SMOOTH
	QVector<SCubicPolynomial> xPol = generateXPolynomials();
	QVector<SCubicPolynomial> yPol = generateYPolynomials();

	QPolygonF p;

	SCubicPolynomial scpX = xPol.at(0);
	SCubicPolynomial scpY = yPol.at(0);
	p.append(QPointF(roundf(scpX.eval(0)), roundf(scpY.eval(0))));
	for(int i=0; i<xPol.size(); i++) {
	  for(int j=1; j<=INTERPOL_STEP; j++) {
		  float u = j / (float) INTERPOL_STEP;
		  SCubicPolynomial scpXU = xPol.at(i);
		  SCubicPolynomial scpYU = yPol.at(i);
		  p.append(QPointF(round(scpXU.eval(u)), round(scpYU.eval(u))));
	  }
	}
	path.addPolygon(p);

#else
	path.moveTo(QPointF(mPoints.at(0).x, mPoints.at(0).y));
	for(int i=1; i<mPoints.size(); i++){
		path.lineTo(QPointF(mPoints.at(i).x, mPoints.at(i).y));
	}
#endif
	setPath(path);
}

QVector<SCubicPolynomial> SStrokeItem::generateXPolynomials(){
	QVector<float> xCoords;
	foreach(sPoint pt, mPoints){
		xCoords << pt.x;
	}
	return generatePolynomials(xCoords);
}

QVector<SCubicPolynomial> SStrokeItem::generateYPolynomials(){
	QVector<float> yCoords;
	foreach(sPoint pt, mPoints){
		yCoords << pt.y;
	}
	return generatePolynomials(yCoords);
}

QVector<SCubicPolynomial> SStrokeItem::generatePolynomials(QVector<float> coords){
	QVector<SCubicPolynomial> polys;
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

	delta[0] = 3*(coords.at(1)-coords.at(0))*gamma[0];
	for(i=1; i<n; i++){
		delta[i] = (3*(coords.at(i+1)-coords.at(i-1))-delta[i-1])*gamma[i];
	}
	delta[n] = (3*(coords.at(n)-coords.at(n-1))-delta[n-1])*gamma[n];

	D[n] = delta[n];
	for(i=n-1; i>=0; i--){
		D[i] = delta[i] - gamma[i]*D[i+1];
	}

	// now compute the coefficients of the cubics
	for(i=0; i < n; i++){
		polys << SCubicPolynomial((float)coords.at(i), D[i], 3*(coords.at(i+1) - coords.at(i)) - 2*D[i] - D[i+1], 2*(coords.at(i) - coords.at(i+1)) + D[i] + D[i+1]);
	}

	return polys;
}


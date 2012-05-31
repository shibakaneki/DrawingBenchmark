/*
 * SStrokeItem.cpp
 *
 *  Created on: May 30, 2012
 *      Author: kindov
 */

#include "SStrokeItem.h"

SStrokeItem::SStrokeItem(const QPen& pen, QGraphicsItem* parent):QGraphicsPathItem(parent){

}

SStrokeItem::~SStrokeItem(){

}

void SStrokeItem::addPoint(sPoint p){
	mPoints << p;
}

void SStrokeItem::smooth(){
	QPainterPath path;

	for(int i=1; i<mPoints.size(); i++){
		QPolygonF poly;
		poly.append(QPointF(mPoints.at(i-1).x, mPoints.at(i-1).y));
		poly.append(QPointF(mPoints.at(i).x, mPoints.at(i).y));
		path.addPolygon(poly);
	}

	setPath(path);
}

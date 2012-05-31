/*
 * SCubicSpline.cpp
 *
 *  Created on: May 25, 2012
 *      Author: kindov
 */
#include <QDebug>
#include "SCubicSpline.h"

SCubicSpline::SCubicSpline(QVector<QPointF> vertexPoints, float distance, float tolerance, float precision){
	mPoints = vertexPoints;
	mPrecision = precision;
	mTolerance = tolerance;
	mDistance = distance;
}

SCubicSpline::~SCubicSpline(){

}

int SCubicSpline::pointsCount(){
	return mPoints.count();
}

QVector<QPointF> SCubicSpline::getAllPoints(){
	QVector<QPointF>interpolatedPoints;

	QPointF p0;
	QPointF p1;
	QPointF p2;
	qreal t, t2, t3, t4;

	qDebug() << "-------------------------------------------------";
	for(int i=0; i<mPoints.size(); i++){
		// p0
		if(0 == i){
			p0.setX(mPoints.at(i).x());
			p0.setY(mPoints.at(i).y());
		}else{

			p0.setX((mPoints.at(i-1).x()+mPoints.at(i).x())/2);
			p0.setY((mPoints.at(i-1).y()+mPoints.at(i).y())/2);
		}

		// p1
		p1.setX(mPoints.at(i).x());
		p1.setY(mPoints.at(i).y());

		// p2
		if(mPoints.size() < i+1){
			p2.setX((mPoints.at(i+1).x()+mPoints.at(i).x())/2);
			p2.setY((mPoints.at(i+1).y()+mPoints.at(i).y())/2);
		}else{
			p2.setX(mPoints.at(i).x());
			p2.setY(mPoints.at(i).y());
		}

		for(float j=0.0; j<1.0; j+=mPrecision){
			t = 1.0-j;
			t2 = t*t;
			t3 = 2.0*j*t;
			t4 = j*j;

			qreal x = t2*p0.x() +t3*p1.x() + t4*p2.x();
			qreal y = t2*p0.y() + t3*p1.y() + t4*p2.y();
			qDebug() << QString("          %0;%1").arg(x).arg(y).toAscii().constData();
			interpolatedPoints << QPointF(x, y);
		}
		qDebug() << mPoints.at(i).x() << ";" << mPoints.at(i).y();
	}

	return interpolatedPoints;
}

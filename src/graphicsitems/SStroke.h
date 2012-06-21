#ifndef SSTROKE_H
#define SSTROKE_H

#include <QList>
#include <QGraphicsLineItem>
#include <QPen>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainterPath>
#include <QGraphicsPolygonItem>

#include "core/SGlobals.h"
#include "maths/SCubicPolynomial.h"
#include "SGraphicsPolygonItem.h"
#include "drawing/SDrawingController.h"


typedef struct{
	float coord;
	qreal pressure;
	qreal rotation;
	int tilt;
	SCubicPolynomial polynomial;
}sCoord;

class SStroke{
public:
	SStroke(QPen p, QGraphicsScene* s);
	virtual ~SStroke();

	void addPoint(sPoint p);
	QGraphicsItem* generateLastSegment();
	void interpolateSegments();

private:
	QList<sCoord> generateXPolynomials();
	QList<sCoord> generateYPolynomials();
	QList<sCoord> generatePolynomials(QList<sCoord> coords);

	QList<sPoint> mPoints;
	QList<sPoint> mInterpolPoints;
	QList<SGraphicsPolygonItem*> mSegments;
	QList<SGraphicsPolygonItem*> mTempSegments;
	QPen mPen;
	QGraphicsScene* mpScene;
	SGraphicsPolygonItem* mpSegment;
	SDrawingController* mpDrawingController;
	sPoint mLastStoredCoord;
};

#endif // SSTROKE_H

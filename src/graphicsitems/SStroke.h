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
	QVector<sCoord> generateXPolynomials();
	QVector<sCoord> generateYPolynomials();
	QVector<sCoord> generatePolynomials(const QVector<sCoord>& coords);
	QList<qreal> generatePressures(qreal before, qreal after, int step);

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

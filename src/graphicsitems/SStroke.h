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
#include "SGraphicsPolygonItem.h"

class SStroke{
public:
	SStroke(QPen p, QGraphicsScene* s);
	virtual ~SStroke();

	void addPoint(sPoint p);
	QGraphicsItem* smooth();

private:
	QList<sPoint> mPoints;
	QList<SGraphicsPolygonItem*> mSegments;
	QPen mPen;
	QGraphicsScene* mpScene;
	SGraphicsPolygonItem* mpSegment;
};

#endif // SSTROKE_H

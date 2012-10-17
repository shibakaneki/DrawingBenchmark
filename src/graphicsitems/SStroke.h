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

#define SMOOTHING			6

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

    void addPoint(sPoint* p);
    void generatePath();

private:
	QList<qreal> generatePressures(qreal before, qreal after, int step);

    QList<sPoint*> mPoints;
	QPen mPen;
	QGraphicsScene* mpScene;
	SGraphicsPolygonItem* mpSegment;
	SDrawingController* mpDrawingController;
    QGraphicsItemGroup* mpPath;
};

#endif // SSTROKE_H

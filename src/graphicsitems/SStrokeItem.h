#ifndef SSTROKEITEM_H
#define SSTROKEITEM_H

#include <QVector>
#include <QPainterPath>
#include "SGraphicsPathItem.h"
#include "maths/SCubicPolynomial.h"

#define INTERPOL_STEP	5

typedef struct{
	qreal width;
	SCubicPolynomial poly;
}sPolyPoint;

typedef struct{
	float coord;
	qreal width;
}sCoordWidth;

class SStrokeItem  : public QGraphicsPathItem{
public:
	SStrokeItem(const QPen& pen, QGraphicsItem* parent=0);
	~SStrokeItem();
	void addPoint(sPoint p);
	void smooth();

private:
	QVector<sPolyPoint> generateXPolynomials();
	QVector<sPolyPoint> generateYPolynomials();
	QVector<sPolyPoint> generatePolynomials(QVector<sCoordWidth> coords);

	QVector<sPoint> mPoints;
	QPainterPath mPath;
};

#endif // SSTROKEITEM_H

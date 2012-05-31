#ifndef SSTROKEITEM_H
#define SSTROKEITEM_H

#include <QVector>

#include "SGraphicsPathItem.h"
#include "maths/SCubicPolynomial.h"

#define INTERPOL_STEP	12

class SStrokeItem  : public QGraphicsPathItem{
public:
	SStrokeItem(const QPen& pen, QGraphicsItem* parent=0);
	~SStrokeItem();
	void addPoint(sPoint p);
	void smooth();

private:
	QVector<SCubicPolynomial> generateXPolynomials();
	QVector<SCubicPolynomial> generateYPolynomials();
	QVector<SCubicPolynomial> generatePolynomials(QVector<float> coords);

	QVector<sPoint> mPoints;
};

#endif // SSTROKEITEM_H

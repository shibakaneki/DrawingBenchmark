#ifndef SGEOMETRYHELPER_H
#define SGEOMETRYHELPER_H

#include <QPolygon>
#include <QPointF>
#include <QLineF>
#include <QPainterPath>

#include "core/SGlobals.h"
#define SMOOTH_PRESSURE	1
#define SINMAX 9000

class SGeometryHelper{
public:
	SGeometryHelper();
	virtual ~SGeometryHelper();
	static QPolygonF lineToPolygon(const sLine& line, qreal baseWidth, bool pressureSensitive = false);
	static void init();
	static qreal sinus(qreal angle);
	static qreal cosinus(qreal angle);

private:
	static QPointF p1a;
	static QPointF p1b;
	static QPointF p2a;
	static QPointF p2b;
	static QLineF pLine;
	static qreal sinslut[SINMAX + 1];
	static qreal cosslut[SINMAX + 1];
};

#endif // SGEOMETRYHELPER_H

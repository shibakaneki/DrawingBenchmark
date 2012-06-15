#ifndef SGEOMETRYHELPER_H
#define SGEOMETRYHELPER_H

#include <QPolygon>
#include <QPointF>
#include <QLineF>
#include <QPainterPath>

#include "core/SGlobals.h"

class SGeometryHelper{
public:
	SGeometryHelper();
	virtual ~SGeometryHelper();
	static QPolygonF lineToPolygon(const sLine& line, qreal baseWidth, bool pressureSensitive = false);

private:
	static QPointF p1a;
	static QPointF p1b;
	static QPointF p2a;
	static QPointF p2b;
	static QLineF pLine;
};

#endif // SGEOMETRYHELPER_H

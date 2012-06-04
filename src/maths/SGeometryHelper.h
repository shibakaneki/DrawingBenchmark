#ifndef SGEOMETRYHELPER_H
#define SGEOMETRYHELPER_H

#include <QPolygon>

#include "core/SGlobals.h"

class SGeometryHelper{
public:
	SGeometryHelper();
	virtual ~SGeometryHelper();
	static QPolygonF lineToPolygon(const sLine& line);

};

#endif // SGEOMETRYHELPER_H

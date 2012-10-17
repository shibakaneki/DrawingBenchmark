#ifndef SDEFAULTBRUSH_H
#define SDEFAULTBRUSH_H

#include "drawing/SBrush.h"

class SDefaultBrush : public SBrush{
public:
	SDefaultBrush();
	virtual ~SDefaultBrush();
    virtual QGraphicsItemGroup* renderPath(QPainterPath path);
};

#endif // SDEFAULTBRUSH_H

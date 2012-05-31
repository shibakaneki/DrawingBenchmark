#ifndef SSTROKEITEM_H
#define SSTROKEITEM_H

#include <QVector>

#include "SGraphicsPathItem.h"

class SStrokeItem  : public QGraphicsPathItem{
public:
	SStrokeItem(const QPen& pen, QGraphicsItem* parent=0);
	~SStrokeItem();
	void addPoint(sPoint p);
	void smooth();

private:
	QVector<sPoint> mPoints;
};

#endif // SSTROKEITEM_H

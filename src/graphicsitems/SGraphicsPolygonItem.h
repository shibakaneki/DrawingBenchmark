#ifndef SGRAPHICSPOLYGONITEM_H
#define SGRAPHICSPOLYGONITEM_H

#include <QGraphicsPolygonItem>
#include <QPolygonF>

#include "core/SGlobals.h"

class SGraphicsPolygonItem : public QGraphicsPolygonItem{
public:
	SGraphicsPolygonItem(const QPolygonF& p, QGraphicsItem* parent=0);
	virtual ~SGraphicsPolygonItem();

	inline void subtract(SGraphicsPolygonItem* p){
		if(boundingRect().intersects(p->boundingRect())){
			QPolygonF subtractedPolygon = polygon().subtracted(p->polygon());
			if(polygon() != subtractedPolygon){
				QGraphicsPolygonItem::setPolygon(subtractedPolygon);
			}
		}
	}

	void setColor(const QColor& c);
};

#endif // SGRAPHICSPOLYGONITEM_H

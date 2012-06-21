#include <QBrush>
#include <QPen>
#include "SGraphicsPolygonItem.h"

SGraphicsPolygonItem::SGraphicsPolygonItem(const QPolygonF& p, QGraphicsItem* parent):QGraphicsPolygonItem(p, parent){

}

SGraphicsPolygonItem::~SGraphicsPolygonItem(){

}

void SGraphicsPolygonItem::setColor(const QColor& c){
	QGraphicsPolygonItem::setBrush(QBrush(c));
	//setPen(Qt::NoPen);
	if(c.alphaF() >= 1.0){
		setPen(Qt::NoPen);
	}else{
		QColor col = c;
		qreal alpha = c.alphaF();
		if (alpha >= 0.2 && alpha < 0.6)
		{
			alpha /= 12;
		}
		else if (alpha < 0.8)
		{
			alpha /= 5;
		}
		else if (alpha < 1.0)
		{
			alpha /= 2;
		}
		col.setAlphaF(alpha);
		QGraphicsPolygonItem::setPen(QPen(col));
	}
}

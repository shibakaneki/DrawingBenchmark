#include "SGraphicsPathItem.h"

SGraphicsPathItem::SGraphicsPathItem(const QPainterPath& path, const QPen& pen, QGraphicsItem *parent):QGraphicsPathItem(path, parent)
{
    mPen = pen;
    QGraphicsPathItem::setPen(mPen);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
}

SGraphicsPathItem::~SGraphicsPathItem()
{

}

void SGraphicsPathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setClipRect(option->exposedRect);
    QGraphicsPathItem::paint(painter, option, widget);
}

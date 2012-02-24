#include "SGraphicsPathItem.h"

SGraphicsPathItem::SGraphicsPathItem(const QPainterPath& path, const QPen& pen, QGraphicsItem *parent):QGraphicsPathItem(path, parent)
{
    mPen = pen;
    QGraphicsPathItem::setPen(mPen);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    mSelectionWidth = 10;
    mSelectionPen.setWidth(mSelectionWidth);
    mSelectionPen.setColor(QColor(0, 0, 0, 40));
}

SGraphicsPathItem::~SGraphicsPathItem()
{

}

void SGraphicsPathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setClipRect(option->exposedRect);

    QStyleOptionGraphicsItem *newOption = new QStyleOptionGraphicsItem(*option);
    newOption->state =QStyle::State_None;
    QGraphicsPathItem::paint(painter, newOption, widget);
    if (option->state & QStyle::State_Selected){
        painter->setPen(mSelectionPen);
        painter->drawRoundedRect(boundingRect(),
                                 mSelectionWidth,
                                 mSelectionWidth);

    }
    else setPen(QPen());

}

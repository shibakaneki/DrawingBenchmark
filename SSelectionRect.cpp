#include <QGraphicsScene>
#include <QPen>
#include "SSelectionRect.h"

SSelectionRect::SSelectionRect(QGraphicsItem* item, QGraphicsItem* parent):QGraphicsRectItem(parent)
  , mpItem(item)
  , mSelected(false)
{
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    hide();
}

SSelectionRect::~SSelectionRect()
{
    if(NULL != mpItem){
        if(NULL != mpItem->scene() && mpItem->scene()->items().contains(this)){
            mpItem->scene()->removeItem(this);
        }
    }
}

void SSelectionRect::setItem(QGraphicsItem *pItem)
{
    mpItem = pItem;
}

void SSelectionRect::toggleSelectionState(bool selected)
{
    if(mSelected != selected){
        mSelected = selected;
        if(!mSelected){
            mpItem->scene()->removeItem(this);
            hide();
        }else{
            if(NULL != mpItem && NULL != mpItem->scene()){
                mpItem->scene()->addItem(this);
                setZValue(mpItem->zValue() - 1);

                setRect(mpItem->boundingRect());
                setVisible(true);
            }
        }
    }
}

void SSelectionRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPen pen;
    pen.setColor(QColor(Qt::lightGray));
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawRoundedRect(boundingRect(), 5, 5);
}

QVariant SSelectionRect::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemSelectedChange){
        prepareGeometryChange();
    }else if(change == ItemSelectedHasChanged){

    }else if(change == ItemTransformHasChanged){

    }else if(change == ItemTransformChange){

    }

    return QGraphicsRectItem::itemChange(change, value);
}

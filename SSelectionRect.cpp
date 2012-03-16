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

    int x = boundingRect().x();
    int y = boundingRect().y();
    int w = boundingRect().width() - GRIPSIZE;
    int h = boundingRect().height() - GRIPSIZE;

    QPen pen;
    pen.setColor(QColor(Qt::blue));
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawRect(x+GRIPSIZE/2, y+GRIPSIZE/2, w, h);

    // Add the grips
    QList<QRect> grips;

    QRect topLeft = QRect(x, y, GRIPSIZE, GRIPSIZE);
    QRect topRight = QRect(x+w, y, GRIPSIZE, GRIPSIZE);
    QRect bottomLeft = QRect(x, y+h, GRIPSIZE, GRIPSIZE);
    QRect bottomRight = QRect(x+w, y+h, GRIPSIZE, GRIPSIZE);

    grips << topLeft;
    grips << topRight;
    grips << bottomLeft;
    grips << bottomRight;

    foreach(QRect grip, grips){
        painter->fillRect(grip,Qt::white);
        painter->drawRect(grip);
    }

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

#include <QImage>
#include <QBrush>
#include <QGraphicsScene>
#include "SGraphicsPathItem.h"

SGraphicsPathItem::SGraphicsPathItem(const QPainterPath& path, const QPen& pen, QGraphicsItem *parent):QGraphicsPathItem(path, parent)
{
    mPen = pen;
    QGraphicsPathItem::setPen(mPen);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    mSelectionWidth = 1;
    mIconSize = 20;
    mSelectionPen.setWidth(mSelectionWidth);
    mSelectionPen.setColor(QColor(0, 0, 255, 255));
    mInitRect = QGraphicsPathItem::boundingRect();
    mWidth = mInitRect.width();
    mHeight = mInitRect.height();
    mX = mInitRect.x();
    mY = mInitRect.y();
    QPainterPath pa;
    pa.addRect(boundingRect());
    mSelectionShape = pa;
    mpSelectionRect = new SSelectionRect(this, 0);
}

SGraphicsPathItem::~SGraphicsPathItem()
{

}

QRectF SGraphicsPathItem::boundingRect() const
{
    int selWidth = mSelectionPen.width();
    return QRectF(mX-selWidth-GRIPSIZE/2, mY-selWidth-GRIPSIZE/2, mWidth+2*selWidth+GRIPSIZE, mHeight+2*selWidth+GRIPSIZE);
}

void SGraphicsPathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setClipRect(option->exposedRect);
    painter->setPen(mPen);

    QStyleOptionGraphicsItem *newOption = new QStyleOptionGraphicsItem(*option);
    newOption->state = QStyle::State_None;
    QGraphicsPathItem::paint(painter, newOption, widget);
    if(option->state & QStyle::State_Selected){
    }else{
        painter->setPen(mPen);
    }
}

QVariant SGraphicsPathItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemSelectedChange){
        prepareGeometryChange();
    }
    else if(change == ItemSelectedHasChanged){
        mpSelectionRect->toggleSelectionState(isSelected());
    }else if(change == ItemPositionHasChanged){
        mpSelectionRect->setPos(pos());
    }else if(change == ItemTransformHasChanged){
        //qDebug() << "m11:" << transform().m11() << ", m22:" << transform().m22();
        // TODO :   update the mX, mY, mWidth & mHeight values in order to refresh the boundingRect and thus,
        //          the selection rectangle
        //mX = x();
        //mY = y();
        //mWidth = mInitRect.width()*transform().m11();
        //mHeight = mInitRect.height()*transform().m22();
    }else if(change == ItemTransformChange){
    }

    return QGraphicsPathItem::itemChange(change, value);
}

QPainterPath SGraphicsPathItem::shape() const
{
    if(isSelected()){
        return mSelectionShape;
    }else{
        return QGraphicsPathItem::shape();
    }
}

void SGraphicsPathItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(boundingRect().contains(event->pos())){
        event->accept();
    }
}



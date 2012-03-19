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
    mGripSize = GRIPSIZE;
    mIconSize = 20;
    mSelectionPen.setWidth(mSelectionWidth);
    mSelectionPen.setColor(QColor(0, 0, 255, 255));
    QRectF initRect = QGraphicsPathItem::boundingRect();
    mWidth = initRect.width();
    mHeight = initRect.height();
    mX = initRect.x();
    mY = initRect.y();
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
    return QRectF(mX-selWidth-mGripSize/2, mY-selWidth-mGripSize/2, mWidth+2*selWidth+mGripSize, mHeight+2*selWidth+mGripSize);
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
        qDebug() << "SGraphicsPathItem selection state is now: " << isSelected();
        mpSelectionRect->toggleSelectionState(isSelected());
    }else if(change == ItemPositionHasChanged){
        mpSelectionRect->setPos(pos());
    }else if(change == ItemTransformHasChanged){
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
        qDebug() << "SGraphicsPathItem clicked!";
        event->accept();
    }else
    {

    }
}

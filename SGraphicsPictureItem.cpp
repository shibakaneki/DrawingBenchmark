#include <QImage>
#include <QGraphicsScene>
#include "SGraphicsPictureItem.h"

SGraphicsPictureItem::SGraphicsPictureItem(QGraphicsItem *parent):QGraphicsPixmapItem(parent)
{
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    mSelectionWidth = 1;
    mIconSize = 20;
    mGripSize = GRIPSIZE;
    mSelectionPen.setWidth(mSelectionWidth);
    mSelectionPen.setColor(QColor(0, 0, 255, 255));
    QRectF initRect = QGraphicsPixmapItem::boundingRect();
    mWidth = initRect.width();
    mHeight = initRect.height();
    mX = initRect.x();
    mY = initRect.y();
    QPainterPath pa;
    pa.addRect(boundingRect());
    mSelectionShape = pa;
    mpSelectionRect = new SSelectionRect(this, 0);
}

SGraphicsPictureItem::~SGraphicsPictureItem()
{

}

QRectF SGraphicsPictureItem::boundingRect() const
{
    int selWidth = mSelectionPen.width();
    QRectF bRect = QGraphicsPixmapItem::boundingRect();
    int w = bRect.width();
    int h = bRect.height();
    int x = bRect.x();
    int y = bRect.y();
    return QRectF(x-selWidth-mGripSize/2, y-selWidth-mGripSize/2, w+2*selWidth+mGripSize, h+2*selWidth+mGripSize);
}

void SGraphicsPictureItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setClipRect(option->exposedRect);
    painter->setPen(mPen);

    QStyleOptionGraphicsItem *newOption = new QStyleOptionGraphicsItem(*option);
    newOption->state = QStyle::State_None;
    QGraphicsPixmapItem::paint(painter, newOption, widget);
    if(option->state & QStyle::State_Selected){

    }else{
        painter->setPen(mPen);
    }
}

QVariant SGraphicsPictureItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemSelectedChange){
        prepareGeometryChange();
    }else if(change == ItemSelectedHasChanged){
        mpSelectionRect->toggleSelectionState(!isSelected());
    }else if(change == ItemTransformHasChanged){
    }else if(change == ItemTransformChange){
    }

    return QGraphicsPixmapItem::itemChange(change, value);
}

QPainterPath SGraphicsPictureItem::shape() const
{
    if(isSelected()){
        return mSelectionShape;
    }else{
        return QGraphicsPixmapItem::shape();
    }
}

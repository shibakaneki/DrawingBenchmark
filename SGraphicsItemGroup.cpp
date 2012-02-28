#include <QImage>

#include "SGraphicsItemGroup.h"

SGraphicsItemGroup::SGraphicsItemGroup(QGraphicsItem *parent):QGraphicsItemGroup(parent)
{
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    mSelectionWidth = 10;
    mIconSize = 20;
    mSelectionPen.setWidth(mSelectionWidth);
    mSelectionPen.setColor(QColor(0, 0, 0, 40));
    QRectF initRect = QGraphicsItemGroup::boundingRect();
    mWidth = initRect.width();
    mHeight = initRect.height();
    mX = initRect.x();
    mY = initRect.y();
}

SGraphicsItemGroup::~SGraphicsItemGroup()
{

}

QRectF SGraphicsItemGroup::boundingRect() const
{
    int selWidth = mSelectionPen.width();
    return QRectF(mX-selWidth, mY-selWidth- mIconSize, mWidth+2*selWidth, mHeight+2*selWidth+mIconSize);
}

void SGraphicsItemGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setClipRect(option->exposedRect);

    QStyleOptionGraphicsItem *newOption = new QStyleOptionGraphicsItem(*option);
    newOption->state =QStyle::State_None;
    QGraphicsItemGroup::paint(painter, newOption, widget);
    if(option->state & QStyle::State_Selected){
        painter->setPen(mSelectionPen);
        painter->drawRoundedRect(boundingRect().x() + mSelectionWidth/2,
                                 boundingRect().y() + mSelectionWidth/2 +mIconSize,
                                 boundingRect().width() - mSelectionWidth,
                                 boundingRect().height() - mSelectionWidth -mIconSize,
                                 mSelectionWidth,
                                 mSelectionWidth);

//        painter->drawImage(boundingRect().x() + boundingRect().width() - mSelectionWidth/2 - mIconSize,
//                           boundingRect().y() + mIconSize/2,
//                           QImage(":/res/close.svg"));

    }
}


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
    mGripSize = 6;
    mSelectionPen.setWidth(mSelectionWidth);
    mSelectionPen.setColor(QColor(0, 0, 255, 255));
    QRectF initRect = QGraphicsPixmapItem::boundingRect();
    mWidth = initRect.width();
    mHeight = initRect.height();
    mX = initRect.x();
    mY = initRect.y();
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
        painter->setPen(mSelectionPen);
        painter->drawRoundedRect(boundingRect().x() + mSelectionWidth/2 + mGripSize/2,
                                 boundingRect().y() + mSelectionWidth/2 + mGripSize/2,
                                 boundingRect().width() - mSelectionWidth -mGripSize,
                                 boundingRect().height() - mSelectionWidth -mGripSize,
                                 mSelectionWidth,
                                 mSelectionWidth);

        painter->setBrush(QBrush(QColor(Qt::white)));
        painter->drawRect(boundingRect().x(), boundingRect().y(), mGripSize, mGripSize);
        painter->drawRect(boundingRect().x(), boundingRect().y() + boundingRect().height()-mGripSize -mSelectionWidth, mGripSize, mGripSize);
        painter->drawRect(boundingRect().x() + boundingRect().width()-mGripSize -mSelectionWidth, boundingRect().y(), mGripSize, mGripSize);
        painter->drawRect(boundingRect().x() + boundingRect().width()-mGripSize -mSelectionWidth, boundingRect().y() + boundingRect().height()-mGripSize -mSelectionWidth, mGripSize, mGripSize);

//        painter->drawImage(boundingRect().x() + boundingRect().width() - mSelectionWidth/2 - mIconSize,
//                           boundingRect().y() + mIconSize/2,
//                           QImage(":/res/close.svg"));

    }else{
        painter->setPen(mPen);
    }
}


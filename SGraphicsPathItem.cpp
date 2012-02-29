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

    mSelectionWidth = 1;
    mGripSize = 6;
    mIconSize = 20;
    mSelectionPen.setWidth(mSelectionWidth);
    mSelectionPen.setColor(QColor(0, 0, 255, 255));
    QRectF initRect = QGraphicsPathItem::boundingRect();
    mWidth = initRect.width();
    mHeight = initRect.height();
    mX = initRect.x();
    mY = initRect.y();
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

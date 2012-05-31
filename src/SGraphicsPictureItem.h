#ifndef SGRAPHICSPICTUREITEM_H
#define SGRAPHICSPICTUREITEM_H

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QRectF>
#include <QPainterPath>
#include <QVariant>

#include "core/SGlobals.h"
#include "SGraphicsItem.h"

class SGraphicsPictureItem : public QGraphicsPixmapItem, public SGraphicsItem
{
public:
    SGraphicsPictureItem(QGraphicsItem* parent=0);
    ~SGraphicsPictureItem();
    QRectF boundingRect() const;
    QPainterPath shape() const;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent* ev);

private:
    QPen mPen;
    QPen mSelectionPen;
    int mSelectionWidth;
    int mWidth;
    int mHeight;
    int mX;
    int mY;
    int mIconSize;
    int mGripSize;
    QPainterPath mSelectionShape;
};

#endif // SGRAPHICSPICTUREITEM_H

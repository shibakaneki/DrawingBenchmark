#ifndef SGRAPHICSPATHITEM_H
#define SGRAPHICSPATHITEM_H

#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QPainterPath>
#include <QPen>
#include <QRectF>
#include <QGraphicsSceneMouseEvent>
#include <QPainterPath>
#include <QVariant>

#include "SGlobals.h"
#include "SGraphicsItem.h"

class SGraphicsPathItem : public QGraphicsPathItem, public SGraphicsItem
{
public:
    SGraphicsPathItem(const QPainterPath& path, const QPen& pen, QGraphicsItem* parent=0);
    ~SGraphicsPathItem();
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
    QPainterPath mSelectionShape;
    QRectF mInitRect;
};

#endif // SGRAPHICSPATHITEM_H

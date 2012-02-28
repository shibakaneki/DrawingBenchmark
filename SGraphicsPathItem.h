#ifndef SGRAPHICSPATHITEM_H
#define SGRAPHICSPATHITEM_H

#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QPainterPath>
#include <QPen>
#include <QRectF>

#include "SGlobals.h"

class SGraphicsPathItem : public QGraphicsPathItem
{
public:
    SGraphicsPathItem(const QPainterPath& path, const QPen& pen, QGraphicsItem* parent=0);
    ~SGraphicsPathItem();
    QRectF boundingRect() const;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QPen mPen;
    QPen mSelectionPen;
    int mSelectionWidth;
    int mWidth;
    int mHeight;
    int mX;
    int mY;
};

#endif // SGRAPHICSPATHITEM_H

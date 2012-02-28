#ifndef SGRAPHICSITEMGROUP_H
#define SGRAPHICSITEMGROUP_H

#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QRectF>

#include "SGlobals.h"

class SGraphicsItemGroup : public QGraphicsItemGroup
{
public:
    SGraphicsItemGroup(QGraphicsItem* parent=0);
    ~SGraphicsItemGroup();
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
    int mIconSize;
};
#endif // SGRAPHICSITEMGROUP_H

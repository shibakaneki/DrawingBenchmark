#ifndef SSELECTIONRECT_H
#define SSELECTIONRECT_H

#include "SGlobals.h"
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QVariant>
#include <QGraphicsSceneMouseEvent>
#include <QRect>

typedef enum{
    eGrip_TopLeft,
    eGrip_TopRight,
    eGrip_BottomLeft,
    eGrip_BottomRight
}eGrip;

class SSelectionRect : public QGraphicsRectItem
{
public:
    SSelectionRect(QGraphicsItem* item=0, QGraphicsItem* parent=0);
    ~SSelectionRect();
    void setItem(QGraphicsItem* pItem);
    void toggleSelectionState(bool selected);
    QGraphicsItem* item();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent* ev);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *ev);

private:
    void updateGripsPosition();
    QRect generateRect();

    QGraphicsItem* mpItem;
    bool mSelected;
    QRect topLeft;
    QRect topRight;
    QRect bottomLeft;
    QRect bottomRight;
    bool mResizing;
};

#endif // SSELECTIONRECT_H

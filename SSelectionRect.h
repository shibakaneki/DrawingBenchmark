#ifndef SSELECTIONRECT_H
#define SSELECTIONRECT_H

#include "SGlobals.h"
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QVariant>

class SSelectionRect : public QGraphicsRectItem
{
public:
    SSelectionRect(QGraphicsItem* item=0, QGraphicsItem* parent=0);
    ~SSelectionRect();
    void setItem(QGraphicsItem* pItem);
    void toggleSelectionState(bool selected);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    QGraphicsItem* mpItem;
    bool mSelected;
};

#endif // SSELECTIONRECT_H

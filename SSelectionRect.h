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
    eGrip_None,
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
    bool resizGripClicked();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent* ev);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *ev);

private:
    void updateGripsPosition();
    QRect generateRect();
    void initializeTransform();
    QTransform buildTransform();

    QGraphicsItem* mpItem;
    bool mSelected;
    QRect topLeft;
    QRect topRight;
    QRect bottomLeft;
    QRect bottomRight;
    bool mResizing;
    eGrip mSelectedGrip;
    qreal mAngle;
    qreal mTotalScaleX;
    qreal mTotalScaleY;
    qreal mScaleX;
    qreal mScaleY;
    qreal mTranslateX;
    qreal mTranslateY;
    qreal mAngleOffset;
    qreal mTotalTranslateX;
    qreal mTotalTranslateY;
    QTransform mInitialTransform;
    QPointF mStartingPoint;
};

#endif // SSELECTIONRECT_H

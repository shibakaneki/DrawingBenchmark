#include <QGraphicsScene>
#include <QPen>
#include "SSelectionRect.h"

SSelectionRect::SSelectionRect(QGraphicsItem* item, QGraphicsItem* parent):QGraphicsRectItem(parent)
  , mpItem(item)
  , mSelected(false)
{
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    hide();
}

SSelectionRect::~SSelectionRect()
{
    if(NULL != mpItem){
        if(NULL != mpItem->scene() && mpItem->scene()->items().contains(this)){
            mpItem->scene()->removeItem(this);
        }
    }
}

void SSelectionRect::setItem(QGraphicsItem *pItem)
{
    mpItem = pItem;
}

void SSelectionRect::toggleSelectionState(bool selected)
{
    if(mSelected != selected){
        mSelected = selected;
        if(!mSelected){
            mpItem->scene()->removeItem(this);
            hide();
        }else{
            if(NULL != mpItem && NULL != mpItem->scene()){
                mpItem->scene()->addItem(this);
                setZValue(mpItem->zValue() - 1);
                setRect(generateRect());
                //setRect(mpItem->boundingRect());
                setVisible(true);
            }
        }
    }
}

QRect SSelectionRect::generateRect()
{
    QRect r;

    if(NULL != mpItem){
        r.setX(mpItem->boundingRect().x()-GRIPSIZE);
        r.setY(mpItem->boundingRect().y()-GRIPSIZE);
        r.setWidth(mpItem->boundingRect().width()+2*GRIPSIZE);
        r.setHeight(mpItem->boundingRect().height()+2*GRIPSIZE);
    }

    return r;
}

void SSelectionRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    int x = boundingRect().x();
    int y = boundingRect().y();
    int w = boundingRect().width() - GRIPSIZE;
    int h = boundingRect().height() - GRIPSIZE;

    QPen pen;
    pen.setColor(QColor(Qt::blue));
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawRect(x+GRIPSIZE/2, y+GRIPSIZE/2, w, h);

    // Add the grips
    updateGripsPosition();

    QList<QRect> grips;
    grips << topLeft;
    grips << topRight;
    grips << bottomLeft;
    grips << bottomRight;

    foreach(QRect grip, grips){
        painter->fillRect(grip,Qt::white);
        painter->drawRect(grip);
    }

}

QGraphicsItem* SSelectionRect::item()
{
    return mpItem;
}

QVariant SSelectionRect::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemSelectedChange){
        prepareGeometryChange();
    }else if(change == ItemSelectedHasChanged){

    }else if(change == ItemTransformHasChanged){

    }else if(change == ItemTransformChange){

    }

    return QGraphicsRectItem::itemChange(change, value);
}

void SSelectionRect::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
    qDebug() << "selection rect clicked!";
    mResizing = false;
    if(topLeft.contains(ev->pos().x(), ev->pos().y())){
        qDebug() << "topleft";
    }else if(topRight.contains(ev->pos().x(), ev->pos().y())){
        qDebug() << "topRight";
    }else if(bottomLeft.contains(ev->pos().x(), ev->pos().y())){
        qDebug() << "bottom left";
    }else if(bottomRight.contains(ev->pos().x(), ev->pos().y())){
        qDebug() << "bottom right";
    }
    ev->accept();
}

void SSelectionRect::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)
{

}

void SSelectionRect::updateGripsPosition()
{
    int x = boundingRect().x();
    int y = boundingRect().y();
    int w = boundingRect().width() - GRIPSIZE;
    int h = boundingRect().height() - GRIPSIZE;
    topLeft = QRect(x, y, GRIPSIZE, GRIPSIZE);
    topRight = QRect(x+w, y, GRIPSIZE, GRIPSIZE);
    bottomLeft = QRect(x, y+h, GRIPSIZE, GRIPSIZE);
    bottomRight = QRect(x+w, y+h, GRIPSIZE, GRIPSIZE);
}

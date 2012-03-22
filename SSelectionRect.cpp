#include <QGraphicsScene>
#include <QPen>
#include <QTransform>

#include "math.h"
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

    int x = mpItem->boundingRect().x();
    int y = mpItem->boundingRect().y();
    int w = mpItem->boundingRect().width() - GRIPSIZE;
    int h = mpItem->boundingRect().height() - GRIPSIZE;

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

bool SSelectionRect::resizGripClicked()
{
    if(eGrip_TopLeft == mSelectedGrip || eGrip_TopRight == mSelectedGrip || eGrip_BottomLeft == mSelectedGrip || eGrip_BottomRight == mSelectedGrip){
        return true;
    }else{
        return false;
    }
}

void SSelectionRect::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
    mStartingPoint = ev->scenePos();
    mResizing = false;
    mSelectedGrip = eGrip_None;

    initializeTransform();
    mScaleX = 1;
    mScaleY = 1;
    mTranslateX = 0;
    mTranslateY = 0;
    mAngleOffset = 0;

    mInitialTransform = buildTransform();

    if(topLeft.contains(ev->pos().x(), ev->pos().y())){
        mSelectedGrip = eGrip_TopLeft;
    }else if(topRight.contains(ev->pos().x(), ev->pos().y())){
        mSelectedGrip = eGrip_TopRight;
    }else if(bottomLeft.contains(ev->pos().x(), ev->pos().y())){
        mSelectedGrip = eGrip_BottomLeft;
    }else if(bottomRight.contains(ev->pos().x(), ev->pos().y())){
        mSelectedGrip = eGrip_BottomRight;
    }
    ev->accept();
}

void SSelectionRect::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)
{
    QLineF move(mStartingPoint, ev->scenePos());
    qreal moveX = move.length() * cos((move.angle() - mAngle) * 3.14 / 180);
    qreal moveY = -move.length() * sin((move.angle() - mAngle) * 3.14 / 180);
    qreal width = mpItem->boundingRect().width() * mTotalScaleX;
    qreal height = mpItem->boundingRect().height() * mTotalScaleY;

    qreal scaleX = 0.0;
    qreal scaleY = 0.0;

    switch(mSelectedGrip){
    case eGrip_TopLeft:
        scaleX = (width - moveX) / width;
        scaleY = (height - moveY) / height;
        mScaleX = scaleX;
        mScaleY = scaleY;
        break;
    case eGrip_TopRight:
        scaleX = (width + moveX) / width;
        scaleY = (height - moveY) / height;
        mScaleX = scaleX;
        mScaleY = scaleY;
        break;
    case eGrip_BottomLeft:
        scaleX = (width - moveX) / width;
        scaleY = (height + moveY) / height;
        mScaleX = scaleX;
        mScaleY = scaleY;
        break;
    case eGrip_BottomRight:
        scaleX = (width + moveX) / width;
        scaleY = (height + moveY) / height;
        mScaleX = scaleX;
        mScaleY = scaleY;
        break;
    default:
        break;
    }

    QTransform tr = buildTransform();
    if(eGrip_BottomRight == mSelectedGrip)
    {
        QPointF topLeft = tr.map(mpItem->boundingRect().topLeft());
        QPointF fixedPoint = mInitialTransform.map(mpItem->boundingRect().topLeft());
        mTranslateX += fixedPoint.x() - topLeft.x();
        mTranslateY += fixedPoint.y() - topLeft.y();
        tr = buildTransform();
    }else if(eGrip_TopLeft == mSelectedGrip){
        QPointF bottomRight = tr.map(mpItem->boundingRect().bottomRight());
        QPointF fixedPoint = mInitialTransform.map(mpItem->boundingRect().bottomRight());
        mTranslateX += fixedPoint.x() - bottomRight.x();
        mTranslateY += fixedPoint.y() - bottomRight.y();
    }else if(eGrip_BottomLeft == mSelectedGrip){
        QPointF topRight = tr.map(mpItem->boundingRect().topRight());
        QPointF fixedPoint = mInitialTransform.map(mpItem->boundingRect().topRight());
        mTranslateX += fixedPoint.x() - topRight.x();
        mTranslateY += fixedPoint.y() - topRight.y();
    }else if(eGrip_TopRight == mSelectedGrip){
        QPointF bottomLeft = tr.map(mpItem->boundingRect().bottomLeft());
        QPointF fixedPoint = mInitialTransform.map(mpItem->boundingRect().bottomLeft());
        mTranslateX += fixedPoint.x() - bottomLeft.x();
        mTranslateY += fixedPoint.y() - bottomLeft.y();
        tr = buildTransform();
    }
    mpItem->setTransform(tr);
    setRect(mpItem->boundingRect());
    update();
}

void SSelectionRect::updateGripsPosition()
{
    int x = rect().x();
    int y = rect().y();
    int w = rect().width() - GRIPSIZE;
    int h = rect().height() - GRIPSIZE;
    topLeft = QRect(x, y, GRIPSIZE, GRIPSIZE);
    topRight = QRect(x+w, y, GRIPSIZE, GRIPSIZE);
    bottomLeft = QRect(x, y+h, GRIPSIZE, GRIPSIZE);
    bottomRight = QRect(x+w, y+h, GRIPSIZE, GRIPSIZE);
}

void SSelectionRect::initializeTransform()
{
    QTransform itemTransform = mpItem->sceneTransform();
    QRectF itemRect = mpItem->boundingRect();
    QPointF topLeft = itemTransform.map(itemRect.topLeft());
    QPointF topRight = itemTransform.map(itemRect.topRight());
    QPointF  bottomLeft = itemTransform.map(itemRect.bottomLeft());

    QLineF topLine(topLeft, topRight);
    QLineF leftLine(topLeft, bottomLeft);

    qreal width = topLine.length();
    qreal height = leftLine.length();

    mAngle = topLine.angle();
    mTotalScaleX = width / itemRect.width();
    mTotalScaleY = height / itemRect.height();

    QTransform tr;
    QPointF center = mpItem->boundingRect().center();
    tr.translate(center.x() * mTotalScaleX, center.y() * mTotalScaleY);
    tr.rotate(-mAngle);
    tr.translate(-center.x() * mTotalScaleX, -center.y() * mTotalScaleY);
    tr.scale(mTotalScaleX, mTotalScaleY);

    mTotalTranslateX = mpItem->transform().dx() - tr.dx();
    mTotalTranslateY = mpItem->transform().dy() - tr.dy();
}

QTransform SSelectionRect::buildTransform()
{
    QTransform tr;
    QPointF center = mpItem->boundingRect().center();
    tr.translate(mTotalTranslateX + mTranslateX, mTotalTranslateY + mTranslateY);
    tr.translate(center.x() * mTotalScaleX * mScaleX, center.y() * mTotalScaleY * mScaleY);
    tr.rotate(-mAngle);
    tr.translate(-center.x() * mTotalScaleX * mScaleX, -center.y() * mTotalScaleY * mScaleY);
    tr.scale(mTotalScaleX * mScaleX, mTotalScaleY * mScaleY);
    return tr;
}

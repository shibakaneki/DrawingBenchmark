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

    if(NULL != item){
        QRectF itemBR = item->boundingRect();
        mTL = itemBR.topLeft();
        mTR = itemBR.topRight();
        mBL = itemBR.bottomLeft();
        mBR = itemBR.bottomRight();
    }

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

QRectF SSelectionRect::boundingRect() const
{
    qreal x = mpItem->boundingRect().x() - SELECTION_WIDTH - GRIPSIZE/2;
    qreal y = mpItem->boundingRect().y() - SELECTION_WIDTH - GRIPSIZE/2;
    qreal w = mpItem->boundingRect().width() + GRIPSIZE;
    qreal h = mpItem->boundingRect().height() + GRIPSIZE;
    return QRectF(x, y, w, h);
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

    prepareGeometryChange();

    int x = boundingRect().x() + GRIPSIZE/2;
    int y = boundingRect().y() + GRIPSIZE/2;
    int w = mpItem->boundingRect().width();
    int h = mpItem->boundingRect().height();

    QPen pen;
    pen.setColor(QColor(Qt::blue));
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawRect(x, y, w, h);

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
    qreal moveX = move.length() * cos((move.angle() - mAngle) * PI/ 180);
    qreal moveY = -move.length() * sin((move.angle() - mAngle) * PI / 180);
    qreal width = mpItem->boundingRect().width() * mTotalScaleX;
    qreal height = mpItem->boundingRect().height() * mTotalScaleY;

    qreal scaleX = 0.0;
    qreal scaleY = 0.0;

    if(0.0 != width && 0.0 != height){
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
            mTL = tr.map(mpItem->boundingRect().topLeft());
            QPointF fixedPoint = mInitialTransform.map(mpItem->boundingRect().topLeft());
            mTranslateX += fixedPoint.x() - mTL.x();
            mTranslateY += fixedPoint.y() - mTL.y();
            tr = buildTransform();
        }else if(eGrip_TopLeft == mSelectedGrip){
            mBR = tr.map(mpItem->boundingRect().bottomRight());
            QPointF fixedPoint = mInitialTransform.map(mpItem->boundingRect().bottomRight());
            mTranslateX += fixedPoint.x() - mBR.x();
            mTranslateY += fixedPoint.y() - mBR.y();
        }else if(eGrip_BottomLeft == mSelectedGrip){
            mTR = tr.map(mpItem->boundingRect().topRight());
            QPointF fixedPoint = mInitialTransform.map(mpItem->boundingRect().topRight());
            mTranslateX += fixedPoint.x() - mTR.x();
            mTranslateY += fixedPoint.y() - mTR.y();
        }else if(eGrip_TopRight == mSelectedGrip){
            mBL = tr.map(mpItem->boundingRect().bottomLeft());
            QPointF fixedPoint = mInitialTransform.map(mpItem->boundingRect().bottomLeft());
            mTranslateX += fixedPoint.x() - mBL.x();
            mTranslateY += fixedPoint.y() - mBL.y();
        }
        mpItem->setTransform(tr);
        update();
    }
}

void SSelectionRect::updateGripsPosition()
{
    int x = boundingRect().x();
    int y = boundingRect().y();
    int w = boundingRect().width();
    int h = boundingRect().height();
    topLeft = QRect(x, y, GRIPSIZE, GRIPSIZE);
    topRight = QRect(x+w-GRIPSIZE, y, GRIPSIZE, GRIPSIZE);
    bottomLeft = QRect(x, y+h-GRIPSIZE, GRIPSIZE, GRIPSIZE);
    bottomRight = QRect(x+w-GRIPSIZE, y+h-GRIPSIZE, GRIPSIZE, GRIPSIZE);
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

sSelectionGeometry SSelectionRect::geometry()
{
    sSelectionGeometry geo;

    geo.topLeft = mTL;
    geo.topRight = mTR;
    geo.bottomLeft = mBL;
    geo.bottomRight = mBR;
    geo.width = mTR.x() - mTL.x();
    geo.height = mBL.y() - mTL.y();

    return geo;
}

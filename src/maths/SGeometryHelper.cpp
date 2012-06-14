
#include <QLineF>
#include <QPainterPath>

#include <math.h>

#include "SGeometryHelper.h"

QPointF SGeometryHelper::p1a;
QPointF SGeometryHelper::p1b;
QPointF SGeometryHelper::p2a;
QPointF SGeometryHelper::p2b;
QLineF SGeometryHelper::pLine;

SGeometryHelper::SGeometryHelper(){

}

SGeometryHelper::~SGeometryHelper(){

}

QPolygonF SGeometryHelper::lineToPolygon(const sLine& line)
{
    qreal x1 = line.p1.x;
    qreal y1 = line.p1.y;
    qreal x2 = line.p2.x;
    qreal y2 = line.p2.y;

    pLine.setP1(QPointF(x1, y1));
    pLine.setP2(QPointF(x2, y2));

    qreal alpha = (90.0 - pLine.angle()) * PI / 180.0;
    qreal hypothenuse = line.p1.lineWidth / 2;

    qreal opposite = sin(alpha) * hypothenuse;
    qreal adjacent = cos(alpha) * hypothenuse;

    p1a.setX(x1 - adjacent);
    p1a.setY(y1 - opposite);
    p1b.setX(x1 + adjacent);
    p1b.setY(y1 + opposite);
    p2a.setX(x2 - adjacent);
	p2a.setY(y2 - opposite);
	p2b.setX(x2 + adjacent);
	p2b.setY(y2 + opposite);

    QPainterPath painterPath;
    painterPath.moveTo(p1a);
    painterPath.lineTo(p2a);
    painterPath.arcTo(x2 - hypothenuse, y2 - hypothenuse, line.p1.lineWidth, line.p1.lineWidth, (90.0 + pLine.angle()), -180.0);
    painterPath.lineTo(p1b);
    painterPath.arcTo(x1 - hypothenuse, y1 - hypothenuse, line.p1.lineWidth, line.p1.lineWidth, -1 * (90.0 - pLine.angle()), -180.0);
    painterPath.closeSubpath();
    return painterPath.toFillPolygon();
}

QPolygonF SGeometryHelper::lineToPolygon(const QPointF& pStart, const QPointF& pEnd,
        const qreal& pStartWidth, const qreal& pEndWidth)
{

    qreal x1 = pStart.x();
    qreal y1 = pStart.y();

    qreal x2 = pEnd.x();
    qreal y2 = pEnd.y();

    QLineF line(pStart, pEnd);

    qreal alpha = (90.0 - line.angle()) * PI / 180.0;
    qreal hypothenuseStart = pStartWidth / 2;

    qreal hypothenuseEnd = pEndWidth / 2;

    qreal sinAlpha = sin(alpha);
    qreal cosAlpha = cos(alpha);

    // TODO UB 4.x PERF cache sin/cos table
    qreal oppositeStart = sinAlpha * hypothenuseStart;
    qreal adjacentStart = cosAlpha * hypothenuseStart;

    QPointF p1a(x1 - adjacentStart, y1 - oppositeStart);
    QPointF p1b(x1 + adjacentStart, y1 + oppositeStart);

    qreal oppositeEnd = sinAlpha * hypothenuseEnd;
    qreal adjacentEnd = cosAlpha * hypothenuseEnd;

    QPointF p2a(x2 - adjacentEnd, y2 - oppositeEnd);

    QPainterPath painterPath;

    painterPath.moveTo(p1a);
    painterPath.lineTo(p2a);

    painterPath.arcTo(x2 - hypothenuseEnd, y2 - hypothenuseEnd, pEndWidth, pEndWidth, (90.0 + line.angle()), -180.0);

    painterPath.lineTo(p1b);

    painterPath.arcTo(x1 - hypothenuseStart, y1 - hypothenuseStart, pStartWidth, pStartWidth, -1 * (90.0 - line.angle()), -180.0);

    painterPath.closeSubpath();

    return painterPath.toFillPolygon();
}

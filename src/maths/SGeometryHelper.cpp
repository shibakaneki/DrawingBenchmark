
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

QPolygonF SGeometryHelper::lineToPolygon(const sLine& line, qreal baseWidth, bool pressureSensitive)
{
    qreal x1 = line.p1.x;
    qreal y1 = line.p1.y;
    qreal x2 = line.p2.x;
    qreal y2 = line.p2.y;
    qreal w1 = pressureSensitive ? line.p1.pressure * baseWidth : baseWidth;
    qreal w2 = pressureSensitive ? line.p2.pressure * baseWidth : baseWidth;

    pLine.setP1(QPointF(x1, y1));
    pLine.setP2(QPointF(x2, y2));

    qreal alpha = (90.0 - pLine.angle()) * PI / 180.0;
    qreal hypothenuse = w1 / 2;

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
    painterPath.arcTo(x2 - hypothenuse, y2 - hypothenuse, w1, w1, (90.0 + pLine.angle()), -180.0);
    painterPath.lineTo(p1b);
    painterPath.arcTo(x1 - hypothenuse, y1 - hypothenuse, w1, w1, -1 * (90.0 - pLine.angle()), -180.0);
    painterPath.closeSubpath();
    return painterPath.toFillPolygon();
}

#include <QPointF>
#include <QLineF>
#include <QPainterPath>

#include <math.h>

#include "SGeometryHelper.h"

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

    QLineF pLine;
    pLine.setP1(QPointF(x1, y1));
    pLine.setP2(QPointF(x2, y2));

    qreal alpha = (90.0 - pLine.angle()) * PI / 180.0;
    qreal hypothenuse = line.p1.lineWidth / 2;

    qreal opposite = sin(alpha) * hypothenuse;
    qreal adjacent = cos(alpha) * hypothenuse;

    QPointF p1a(x1 - adjacent, y1 - opposite);
    QPointF p1b(x1 + adjacent, y1 + opposite);

    QPointF p2a(x2 - adjacent, y2 - opposite);
    QPointF p2b(x2 + adjacent, y2 + opposite);

    QPainterPath painterPath;

    painterPath.moveTo(p1a);
    painterPath.lineTo(p2a);

    painterPath.arcTo(x2 - hypothenuse, y2 - hypothenuse, line.p1.lineWidth, line.p1.lineWidth, (90.0 + pLine.angle()), -180.0);

    //painterPath.lineTo(p2b);
    painterPath.lineTo(p1b);

    painterPath.arcTo(x1 - hypothenuse, y1 - hypothenuse, line.p1.lineWidth, line.p1.lineWidth, -1 * (90.0 - pLine.angle()), -180.0);

    painterPath.closeSubpath();

    return painterPath.toFillPolygon();
}

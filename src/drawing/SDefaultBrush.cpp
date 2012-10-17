#include "drawing/SDefaultBrush.h"

SDefaultBrush::SDefaultBrush():SBrush(){
	mName = "Default";
	mWidthPressure = true;
}

SDefaultBrush::~SDefaultBrush(){

}

QGraphicsItemGroup* SDefaultBrush::renderPath(QPainterPath path){
    QGraphicsItemGroup* pGroup = new QGraphicsItemGroup();

    QPen pen;
    QColor c1 = mColor;
    QColor c2 = mColor;
    int alpha1 = 3*mColor.alpha()/4;
    c1.setAlpha(alpha1);
    int alpha2 = alpha1*mHardness/100;
    c2.setAlpha(alpha2);
    pen.setColor(QColor(Qt::transparent));
    QRadialGradient gradient(500, 500, 500, 500, 500);
    gradient.setColorAt(0, c1);
    gradient.setColorAt(1, c2);
    QBrush brush(gradient);


    for(int i=0; i<path.length(); i+=(mWidth/2)*mSpacing){
        QGraphicsEllipseItem* pEllipse = new QGraphicsEllipseItem();
        pEllipse->setPen(pen);
        pEllipse->setBrush(brush);
        QPointF pt = path.pointAtPercent(path.percentAtLength(i));
        pEllipse->setRect(pt.x()-mWidth/2, pt.y()-mWidth/2, mWidth, mWidth);
        pGroup->addToGroup(pEllipse);
    }

    return pGroup;
}

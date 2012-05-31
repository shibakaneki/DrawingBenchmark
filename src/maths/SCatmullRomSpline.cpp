#include <math.h>
#include "SCatmullRomSpline.h"

SCatmullRomSpline::SCatmullRomSpline(QVector<QPointF> vertexPoints)
{
    mApproxLineLength = 5;
    mLengths.clear();
    mVertexes = vertexPoints;
}

SCatmullRomSpline::~SCatmullRomSpline()
{

}

int SCatmullRomSpline::pointsCount()
{
    return mNbPoints;
}

QPointF SCatmullRomSpline::catmullRom(double t, QPointF p0, QPointF p1, QPointF p2, QPointF p3)
{
    QPointF p;
    double t2 = t*t;
    double t3 = t2*t;

    qreal x = 0.5 *((2 * p1.x()) + (-p0.x() + p2.x()) * t +(2*p0.x() - 5*p1.x() + 4*p2.x() - p3.x()) * t2 + (-p0.x() + 3*p1.x()- 3*p2.x() + p3.x()) * t3);
    qreal y = 0.5 *((2 * p1.y()) + (-p0.y() + p2.y()) * t +(2*p0.y() - 5*p1.y() + 4*p2.y() - p3.y()) * t2 + (-p0.y() + 3*p1.y()- 3*p2.y() + p3.y()) * t3);
    p.setX(x);
    p.setY(y);
    return p;
}

QVector<QPointF> SCatmullRomSpline::getAllPoints(int approxLineLength)
{
    QVector<QPointF> p;

    if(0 == approxLineLength){
        approxLineLength = mApproxLineLength;
    }

    if(mVertexPlus.empty()){
        makeVertexPlus();
    }

    int len = mVertexes.size();
    for(int i=0; i<len-3; i++){
        QPointF p1 = mVertexPlus.at(i+1);
        QPointF p2 = mVertexPlus.at(i+2);
        QPointF pMid = catmullRom(0.5, mVertexPlus.at(i), p1, p2, mVertexPlus.at(i+3));

        // An approximate midpoint length calculation
        double dist = sqrt(abs(pow(pMid.x()-p1.x() ,2)) + abs(pow(pMid.y() - p1.y() ,2))) + sqrt(abs(pow(pMid.x()-p2.x() ,2)) + abs(pow(pMid.y() - p2.y() ,2)));
        mLengths.append(dist);

        double t = 1.0/(dist/approxLineLength);
        for(double j=0.0; j<1.0; j+=t){
            QPointF point = catmullRom(j, mVertexPlus.at(i), p1, p2, mVertexPlus.at(i+3));
            p.append(point);
        }
    }

    // Replace the last point by the original last point
    if(p.size() > 1 && mVertexPlus.size() > 0){
        p.replace(p.size()-1, mVertexPlus.at(mVertexPlus.size()-1));
    }

    mNbPoints = p.size();

    return p;
}

void SCatmullRomSpline::makeVertexPlus()
{
    mVertexPlus = mVertexes;
    mVertexPlus.insert(0, mVertexPlus.at(0));
    mVertexPlus.append(mVertexPlus.at(mVertexPlus.size()-1));
}

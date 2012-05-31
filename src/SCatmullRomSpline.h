#ifndef SCATMULLROMSPLINE_H
#define SCATMULLROMSPLINE_H

#include <QVector>
#include <QPointF>

#include "core/SGlobals.h"

class SCatmullRomSpline
{
public:
    SCatmullRomSpline(QVector<QPointF> vertexPoints);
    ~SCatmullRomSpline();
    int pointsCount();
    QVector<QPointF> getAllPoints(int approxLineLength);

private:

    QPointF catmullRom(double t, QPointF p0, QPointF p1, QPointF p2, QPointF p3);
    void makeVertexPlus();

    /** An array of vertexes */
    QVector<QPointF> mVertexes;
    /** The distances between neighboring vertexes */
    QVector<double> mLengths;
    /** The total length of the spline */
    int mTotalLength;
    /** The number of points */
    int mNbPoints;
    /** Approximation of the line length */
    int mApproxLineLength;
    /** An array of vertexes, like mVertexes but with double start & end points */
    QVector<QPointF> mVertexPlus;
};

#endif // SCATMULLROMSPLINE_H

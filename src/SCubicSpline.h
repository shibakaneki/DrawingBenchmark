#ifndef SCUBICSPLINE_H
#define SCUBICSPLINE_H

#include <QVector>
#include <QPointF>

class SCubicSpline{

public:
	SCubicSpline(QVector<QPointF> vertexPoints, float distance, float tolerance, float precision);
	~SCubicSpline();
	int pointsCount();
	QVector<QPointF> getAllPoints();

private:
	float mDistance;
	float mTolerance;
	float mPrecision;
	QVector<QPointF> mPoints;
};

#endif // SCUBICSPLINE_H

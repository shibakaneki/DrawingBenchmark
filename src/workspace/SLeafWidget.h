/*
 * SLeafWidget.h
 *
 *  Created on: Jun 4, 2012
 *      Author: kindov
 */

#ifndef SLEAFWIDGET_H_
#define SLEAFWIDGET_H_

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTabletEvent>
#include <QMouseEvent>
#include <QWheelEvent>

#include "core/SGlobals.h"
#include "drawing/SDrawingController.h"

#define MAX_ZOOM_LEVEL 40

class SLeafWidget : public QGraphicsView{
public:
	SLeafWidget(QWidget* parent=0, const char* name=ON_LEAFWIDGET);
	virtual ~SLeafWidget();

protected:
	void tabletEvent(QTabletEvent* ev);
	void mousePressEvent(QMouseEvent* ev);
	void mouseMoveEvent(QMouseEvent* ev);
	void mouseReleaseEvent(QMouseEvent* ev);
	void wheelEvent(QWheelEvent* ev);

private:
	void setMouseEventValues(QMouseEvent* ev);
	void zoomIn(QPointF p);
	void zoomOut(QPointF p);
	bool performPressEvent(QPointF p);
	bool performMoveEvent(QPointF p);
	bool performReleaseEvent(QPointF p);
	void setCenter(const QPointF& centerPoint);

    QGraphicsScene* mpScene;
    SDrawingController* mpDrawingController;
    float mScaleFactor;
    int mZoomDepth;
    QPointF mLastPanPoint;
    QPointF mCurrentCenterPoint;
};

#endif /* SLEAFWIDGET_H_ */

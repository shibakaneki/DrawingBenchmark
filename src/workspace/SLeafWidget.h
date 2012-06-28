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

#include "core/SGlobals.h"
#include "drawing/SDrawingController.h"

class SLeafWidget : public QGraphicsView{
public:
	SLeafWidget(QWidget* parent=0, const char* name=ON_LEAFWIDGET);
	virtual ~SLeafWidget();

protected:
	void tabletEvent(QTabletEvent* ev);
	void mousePressEvent(QMouseEvent* ev);
	void mouseMoveEvent(QMouseEvent* ev);
	void mouseReleaseEvent(QMouseEvent* ev);

private:
	void setMouseEventValues(QMouseEvent* ev);

    QGraphicsScene* mpScene;
    SDrawingController* mpDrawingController;
};

#endif /* SLEAFWIDGET_H_ */

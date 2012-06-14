/*
 * SLeafWidget.h
 *
 *  Created on: Jun 4, 2012
 *      Author: kindov
 */

#ifndef SLEAFWIDGET_H_
#define SLEAFWIDGET_H_

#include <QWidget>
#include <QPaintEvent>

#include "core/SGlobals.h"

#define SQUARE_WIDTH	10

class SLeafWidget : public QWidget{
public:
	SLeafWidget(QWidget* parent=0, const char* name=ON_LEAFWIDGET);
	virtual ~SLeafWidget();

protected:
	virtual void paintEvent(QPaintEvent* ev);
};

#endif /* SLEAFWIDGET_H_ */

/*
 * SLeafWidget.h
 *
 *  Created on: Jun 4, 2012
 *      Author: kindov
 */

#ifndef SLEAFWIDGET_H_
#define SLEAFWIDGET_H_

#include <QWidget>

#include "core/SGlobals.h"

class SLeafWidget : public QWidget{
public:
	SLeafWidget(QWidget* parent=0, const char* name=ON_LEAFWIDGET);
	virtual ~SLeafWidget();
};

#endif /* SLEAFWIDGET_H_ */

#ifndef SBRUSHPREVIEWWIDGET_H
#define SBRUSHPREVIEWWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainterPath>

#include "core/SGlobals.h"

#define PREVIEW_WIDTH		50

class SBrushPreviewWidget : public QWidget{
	Q_OBJECT
public:
	SBrushPreviewWidget(QWidget* parent=0, const char* name="SBrushPreviewWidget");
	virtual ~SBrushPreviewWidget();

protected:
	void paintEvent(QPaintEvent* ev);

private:
	QPainterPath generatePath();
};

#endif // SBRUSHPREVIEWWIDGET_H

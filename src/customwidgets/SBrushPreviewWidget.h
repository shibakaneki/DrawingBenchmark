#ifndef SBRUSHPREVIEWWIDGET_H
#define SBRUSHPREVIEWWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainterPath>
#include <QList>
#include <QPointF>
#include <QResizeEvent>

#include "core/SGlobals.h"
#include "drawing/SDrawingController.h"
#include "drawing/SBrush.h"

typedef struct{
	QPointF p;
	qreal w;
}sCircle;

#define PREVIEW_WIDTH		50
#define PREVIEWPATH_POINTS	100

class SBrushPreviewWidget : public QWidget{
	Q_OBJECT
public:
	SBrushPreviewWidget(QWidget* parent=0, const char* name="SBrushPreviewWidget");
	virtual ~SBrushPreviewWidget();
    void refresh();

protected:
	void paintEvent(QPaintEvent* ev);
	void resizeEvent(QResizeEvent* ev);

private slots:
	void onBrushChanged(SBrush* b);

private:
	void generatePath();

	QList<QPointF> points;
	SBrush* mpBrush;
};

#endif // SBRUSHPREVIEWWIDGET_H

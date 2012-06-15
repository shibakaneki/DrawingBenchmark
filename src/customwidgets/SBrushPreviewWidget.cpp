#include <QPainter>

#include "SBrushPreviewWidget.h"

SBrushPreviewWidget::SBrushPreviewWidget(QWidget* parent, const char* name):QWidget(parent){
	setObjectName(name);
	SETUP_STYLESHEET;
	setMinimumHeight(PREVIEW_WIDTH);
	setMaximumHeight(PREVIEW_WIDTH);
}

SBrushPreviewWidget::~SBrushPreviewWidget(){

}

void SBrushPreviewWidget::paintEvent(QPaintEvent* ev){
	// TODO: get back the current brush informations and display something here
	Q_UNUSED(ev);
	QPainter p;
	QBrush b;
	b.setColor(Qt::white);
	b.setStyle(Qt::SolidPattern);
	p.begin(this);
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setBrush(b);
	p.drawRect(rect());

	QPen pen;
	pen.setWidth(10);
	pen.setCapStyle(Qt::RoundCap);


	p.setPen(Qt::black);
	p.setPen(pen);
	p.drawPath(generatePath());

	p.end();
}

QPainterPath SBrushPreviewWidget::generatePath(){
	QPainterPath path;

	int padding = 10;
	QPointF origin = QPointF(padding, 2*height()/3);
	QPointF dest = QPointF(width()-padding, height()/3);
	QPointF ct1 = QPointF(width()/2, 2*height()/3);
	QPointF ct2 = QPointF(width()/2, height()/3);
	path.moveTo(origin);

	path.cubicTo(ct1, ct2, dest);

	return path;
}

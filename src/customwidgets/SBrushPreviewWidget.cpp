#include <QPainter>

#include "SBrushPreviewWidget.h"

SBrushPreviewWidget::SBrushPreviewWidget(QWidget* parent, const char* name):QWidget(parent){
	setObjectName(name);
	SETUP_STYLESHEET;
	setMinimumHeight(PREVIEW_WIDTH);
	setMaximumHeight(PREVIEW_WIDTH);

	connect(SDrawingController::drawingController(), SIGNAL(brushChanged(SBrush*)), this, SLOT(onBrushChanged(SBrush*)));
}

SBrushPreviewWidget::~SBrushPreviewWidget(){
	DELETEPTR(mpBrush);
}

void SBrushPreviewWidget::paintEvent(QPaintEvent* ev){
	Q_UNUSED(ev);
	QPainter p;
	QBrush b;
	b.setColor(Qt::white);
	b.setStyle(Qt::SolidPattern);

	p.begin(this);
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setBrush(b);
	p.drawRect(rect());
	if(NULL != mpBrush){
		QPen pen;
		pen.setCapStyle(Qt::RoundCap);
		QColor c = mpBrush->color();
		int w = mpBrush->width();

		// First half
		int alphaStep=0;
		float pressureStep = 0.0;
		for(int i=0; i<points.size()/2; i++){
			if(mpBrush->isOpacityPressureSensitive()){
				c.setAlpha(alphaStep);
				alphaStep += 255 / (3+PREVIEWPATH_POINTS / 2);
			}

			pen.setColor(c);
			p.setPen(pen);
			p.setBrush(QBrush(c));

			qreal ellipseWidth = (qreal)w/2;
			if(mpBrush->isWidthPressureSensitive()){
				pressureStep += 1.0/(3+PREVIEWPATH_POINTS / 2);
				ellipseWidth = (qreal)w*pressureStep/2;
			}

			p.drawEllipse(points.at(i), ellipseWidth, ellipseWidth);
		}

		// Second half
		alphaStep = 255;
		pressureStep = 1.0;
		for(int i=points.size()/2; i<points.size(); i++){
			if(mpBrush->isOpacityPressureSensitive()){
				c.setAlpha(alphaStep);
				alphaStep -= 255 / (3+PREVIEWPATH_POINTS / 2);
			}

			pen.setColor(c);
			p.setPen(pen);
			p.setBrush(QBrush(c));

			qreal ellipseWidth = (qreal)w/2;
			if(mpBrush->isWidthPressureSensitive()){
				pressureStep -= 1.0/(3+PREVIEWPATH_POINTS / 2);
				ellipseWidth = (qreal)w*pressureStep/2;
			}

			p.drawEllipse(points.at(i), ellipseWidth, ellipseWidth);
		}
	}
	p.end();
}

void SBrushPreviewWidget::generatePath(){
	points.clear();

	QPainterPath p;
	int padding = 10;
	QPointF origin = QPointF(padding, 2*height()/3);
	QPointF dest = QPointF(width()-padding, height()/3);
	QPointF ct1 = QPointF(width()/2, 2*height()/3);
	QPointF ct2 = QPointF(width()/2, height()/3);
	p.moveTo(origin);
	p.cubicTo(ct1, ct2, dest);

	// Generate the lines
	for(qreal i=0; i<1.0; i+=0.01){
		points << p.pointAtPercent(i);
	}
}

void SBrushPreviewWidget::onBrushChanged(SBrush* b){
	if(NULL != b){
		mpBrush = b;
		generatePath();
		update();
	}
}

void SBrushPreviewWidget::resizeEvent(QResizeEvent* ev){
	Q_UNUSED(ev);
	generatePath();
	update();
}

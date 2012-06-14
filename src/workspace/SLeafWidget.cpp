#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QPixmap>
#include "SLeafWidget.h"

SLeafWidget::SLeafWidget(QWidget* parent, const char* name):QWidget(parent){
	setObjectName(name);
	setAttribute(Qt::WA_TranslucentBackground);
	setStyleSheet("background-color:transparent;");
}

SLeafWidget::~SLeafWidget(){

}

void SLeafWidget::paintEvent(QPaintEvent* ev){
	Q_UNUSED(ev);
	QPainter p;
	QBrush b;
	b.setTexture(QPixmap(":/res/bgtex.png"));
	b.setStyle(Qt::TexturePattern);
	p.begin(this);
	p.setBrush(b);
	p.drawRect(rect());
	p.end();
}

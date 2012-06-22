#include <QApplication>
#include <QDesktopWidget>
#include "SLeafWidget.h"

SLeafWidget::SLeafWidget(QWidget* parent, const char* name):QGraphicsView(parent){
	setObjectName(name);
    setStyleSheet("background:transparent;");

    mpScene = new QGraphicsScene();
    QRect r = QApplication::desktop()->rect();
    mpScene->setSceneRect((r.width()*10)/2, (r.height()*10)/2, r.width()*10, r.height()*10);
    setScene(mpScene);
}

SLeafWidget::~SLeafWidget(){
    DELETEPTR(mpScene);
}

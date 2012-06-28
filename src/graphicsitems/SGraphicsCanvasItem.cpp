#include "SGraphicsCanvasItem.h"
#include <QPushButton>
SGraphicsCanvasItem::SGraphicsCanvasItem():QGraphicsProxyWidget()
	, mpContainer(NULL)
{
	mpContainer = new QWidget();
	setWidget(mpContainer);
}

SGraphicsCanvasItem::~SGraphicsCanvasItem(){
	DELETEPTR(mpContainer);
}

void SGraphicsCanvasItem::resizeCanvas(int w, int h){
	mpContainer->resize(w, h);
}

void SGraphicsCanvasItem::addLayer(SDrawingView* pLayer){
	if(NULL != pLayer){
		//pLayer->setParent(mpContainer);
		//mLayers << pLayer;
		pLayer->resize(mpContainer->size());
		pLayer->setSceneRect(rect());
		setWidget(pLayer);
	}
}

void SGraphicsCanvasItem::mousePressEvent(QGraphicsSceneMouseEvent* ev){

}

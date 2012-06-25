#include "SGraphicsCanvasItem.h"
#include <QPushButton>
SGraphicsCanvasItem::SGraphicsCanvasItem():QGraphicsProxyWidget(){
    setWidget(new QPushButton());
}

SGraphicsCanvasItem::~SGraphicsCanvasItem(){

}

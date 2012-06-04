#include "SLayer.h"

SLayer::SLayer(){
	name = "";
	visible = true;
	layerWidget = new SDrawingView();
}

SLayer::~SLayer(){
	DELETEPTR(layerWidget);
}

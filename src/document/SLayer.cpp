#include "SLayer.h"

SLayer::SLayer(){
	name = "";
	visible = true;
	layerWidget = new SDrawingView();
	mBackground = false;
}

SLayer::~SLayer(){
	DELETEPTR(layerWidget);
}

void SLayer::setAsBackgroundLayer(){
	mBackground = true;
}

bool SLayer::isBackgroundLayer(){
	return mBackground;
}

#include "SLayerPalette.h"

SLayerPalette::SLayerPalette(QWidget* parent, const char* name):SDockPalette(parent){
	Q_UNUSED(name);
	SETUP_STYLESHEET;
	setWindowTitle(tr("Layers"));
}

SLayerPalette::~SLayerPalette(){

}

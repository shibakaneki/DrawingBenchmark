#ifndef SLAYERPALETTE_H
#define SLAYERPALETTE_H

#include "core/SGlobals.h"
#include "dockpalettes/SDockPalette.h"

class SLayerPalette : public SDockPalette{
public:
	SLayerPalette(QWidget* parent=0, const char* name="SLayerWidget");
	virtual ~SLayerPalette();
};

#endif // SLAYERPALETTE_H

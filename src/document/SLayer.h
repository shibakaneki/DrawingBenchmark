#ifndef SLAYER_H
#define SLAYER_H

#include "workspace/SDrawingView.h"
#include <QString>

#include "core/SGlobals.h"

class SLayer{
public:
	SLayer();
	virtual ~SLayer();

	int id;
	QString name;
	bool visible;
	SDrawingView* layerWidget;
};

#endif // SLAYER_H

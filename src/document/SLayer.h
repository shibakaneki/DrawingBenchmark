#ifndef SLAYER_H
#define SLAYER_H

#include "workspace/SDrawingView.h"
#include <QString>

#include "core/SGlobals.h"

class SLayer{
public:
	SLayer();
	virtual ~SLayer();

	void setAsBackgroundLayer();
	bool isBackgroundLayer();

	int id;
	QString name;
	bool visible;
	SDrawingView* layerWidget;

private:
	bool mBackground;
};

#endif // SLAYER_H

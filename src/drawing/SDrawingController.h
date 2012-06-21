#ifndef SDRAWINGCONTROLLER_H
#define SDRAWINGCONTROLLER_H

#include <QObject>
#include <QColor>

#include "core/SGlobals.h"
#include "drawing/SBrush.h"

class SDrawingController : public QObject{
	Q_OBJECT
public:
	SDrawingController();
	virtual ~SDrawingController();
	static SDrawingController* drawingController();
	SBrush* currentBrush();
	void setCurrentBrush(SBrush* b);
	void setWidthPressureSensitive(bool sensitive);
	void setOpacityPressureSensitive(bool sensitive);
	int interpolationStep();
	int interpolationLevel();
	void setInterpolationLevel(int l);
	void setInterpolationStep(int s);

signals:
	void brushChanged(SBrush* b);

public slots:
	void onWidthChanged(int w);
	void onColorChanged(QColor c);

private:
	static SDrawingController* mpInstance;
	SBrush* mpBrush; // The current brush
	int mInterpolStep; // The number of interpolated points between 2 points
	int mInterpolLevel; // The number of points that will be redrawn because of the interpolation
};

#endif // SDRAWINGCONTROLLER_H

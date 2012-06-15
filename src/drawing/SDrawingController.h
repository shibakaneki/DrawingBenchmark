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

signals:
	void brushChanged(SBrush* b);

public slots:
	void onWidthChanged(int w);
	void onColorChanged(QColor c);

private:
	static SDrawingController* mpInstance;
	SBrush* mpBrush; // The current brush
};

#endif // SDRAWINGCONTROLLER_H

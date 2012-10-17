#ifndef SDRAWINGCONTROLLER_H
#define SDRAWINGCONTROLLER_H

#include <QObject>
#include <QColor>
#include <QPointF>

#include "core/SGlobals.h"
#include "drawing/SBrush.h"

typedef enum{
	eInputType_MousePress,
	eInputType_MouseMove,
	eInputType_MouseRelease,
	eInputType_TabletPress,
	eInputType_TabletMove,
	eInputType_TabletRelease
}eInputType;

typedef enum{
    eColorRole_Foreground,
    eColorRole_Background
}eColorRole;

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
    int spacing();
    void setSpacing(int l);
    int hardness();
    void setHardness(int h);
    int width();
    void setWidth(int w);
    QColor foregroundColor();
    void setForegroundColor(QColor c);
    QColor backgroundColor();
    void setBackgroundColor(QColor c);

	qreal pressure;
	qreal rotation;
	qreal tangentialPressure;
	int xTilt;
	int yTilt;
	QPointF point;

signals:
	void brushChanged(SBrush* b);

public slots:
	void onColorChanged(QColor c);

private:
	static SDrawingController* mpInstance;
    SBrush* mpBrush;
    int mSpacing;
    int mHardness;
    QColor mForegroundColor;
    QColor mBackgroundColor;
    eColorRole mCurrentColor;
};

#endif // SDRAWINGCONTROLLER_H

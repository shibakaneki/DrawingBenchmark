#ifndef SBRUSH_H
#define SBRUSH_H

#include <QColor>
#include <QString>
#include <QGraphicsItemGroup>
#include <QPen>

class SBrush{
public:
	SBrush();
	virtual ~SBrush();

	int width();
	void setWidth(int w);
	QColor color();
	void setColor(QColor c);
	QString name();
	void rename(const QString& n);
    int spacing();
    void setSpacing(int s);
    int hardness();
    void setHardness(int h);

    virtual QGraphicsItemGroup* renderPath(QPainterPath path);

	bool isWidthPressureSensitive();
	void setWidthPressureSensitive(bool s);
	bool isOpacityPressureSensitive();
	void setOpacityPressureSensitive(bool s);

protected:
    int mWidth;
    int mSpacing;
    int mHardness;
	QColor mColor;
	QString mName;
	bool mWidthPressure;
	bool mOpacityPressure;
};

#endif // SBRUSH_H

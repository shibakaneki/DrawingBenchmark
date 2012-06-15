#ifndef SBRUSH_H
#define SBRUSH_H

#include <QColor>
#include <QString>

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

	bool isWidthPressureSensitive();
	void setWidthPressureSensitive(bool s);
	bool isOpacityPressureSensitive();
	void setOpacityPressureSensitive(bool s);

protected:
	int mWidth;
	QColor mColor;
	QString mName;
	bool mWidthPressure;
	bool mOpacityPressure;
};

#endif // SBRUSH_H

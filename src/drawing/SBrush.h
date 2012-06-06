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

protected:
	int mWidth;
	QColor mColor;
	QString mName;
};

#endif // SBRUSH_H

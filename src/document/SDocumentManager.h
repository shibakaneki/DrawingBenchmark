#ifndef SDOCUMENTMANAGER_H
#define SDOCUMENTMANAGER_H

#include <QVector>
#include <QString>

#include "SLayer.h"

class SDocumentManager{
public:
	SDocumentManager();
	virtual ~SDocumentManager();

	static void addLayer(const QString& name, QWidget* parent=0);
};

#endif // SDOCUMENTMANAGER_H

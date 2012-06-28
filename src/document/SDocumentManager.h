#ifndef SDOCUMENTMANAGER_H
#define SDOCUMENTMANAGER_H

#include <QList>
#include <QString>

#include "SLayer.h"

class SDocumentManager : public QObject{
	Q_OBJECT
public:
	SDocumentManager();
	virtual ~SDocumentManager();

	SDrawingView* addLayer(const QString& name, QWidget* parent=0);
	static SDocumentManager* documentManager();

	void setDocumentSize(const QSize& s);
	QSize documentSize();

	void forwardEventToLayer(eInputType input);

public slots:
	void onClear();

private:
	static SDocumentManager* mpInstance;
	int mNextLayerID;
	QList<SLayer*> mLayers;
	SLayer* mpCurrentLayer;
	QSize mDocSize;
};

#endif // SDOCUMENTMANAGER_H

#ifndef STOOLSCONTROLLER_H
#define STOOLSCONTROLLER_H

#include <QObject>

#include "core/SGlobals.h"

class SToolsController : public QObject{
	Q_OBJECT
public:
	SToolsController();
	virtual ~SToolsController();
	static SToolsController* toolsController();
	eTool currentTool();

public slots:
	void onSetCurrentTool(eTool tool);

private:
	static SToolsController* mpInstance;
	eTool mCurrentTool;
};

#endif // STOOLSCONTROLLER_H

#ifndef SCHECKBOX_H
#define SCHECKBOX_H

#include <QCheckBox>
#include "core/SGlobals.h"

class SCheckBox : public QCheckBox{
public:
	SCheckBox(QWidget* parent=0, const char* name="SCheckBox");
	virtual ~SCheckBox();
};

#endif // SCHECKBOX_H

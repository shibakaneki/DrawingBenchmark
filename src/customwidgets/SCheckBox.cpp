#include "SCheckBox.h"

SCheckBox::SCheckBox(QWidget* parent, const char* name):QCheckBox(parent){
	setObjectName(name);
	SETUP_STYLESHEET;
}

SCheckBox::~SCheckBox(){

}

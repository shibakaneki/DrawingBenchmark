#include "SDockPalette.h"

SDockPalette::SDockPalette(QWidget *parent, const char *name):QDockWidget(parent){
    SETUP_STYLESHEET
    setObjectName(name);
    setMinimumWidth(200);
}

SDockPalette::~SDockPalette()
{

}

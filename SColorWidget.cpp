#include "SColorWidget.h"

SColorWidget::SColorWidget(QWidget *parent, const char *name):QDockWidget( parent)
{
    setObjectName(name);
    setStyleSheet("background:red;");
}

SColorWidget::~SColorWidget()
{

}

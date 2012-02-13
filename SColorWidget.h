#ifndef SCOLORWIDGET_H
#define SCOLORWIDGET_H

#include <QDockWidget>
#include <QFrame>
#include <QSlider>
#include <QLabel>

#include "SGlobals.h"

class SColorWidget : public QDockWidget
{
    Q_OBJECT
public:
    SColorWidget(QWidget* parent=0, const char* name="SColorWidget");
    ~SColorWidget();

private:

};

#endif // SCOLORWIDGET_H

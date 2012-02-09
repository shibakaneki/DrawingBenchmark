#ifndef SMAINWND_H
#define SMAINWND_H

#include <QMainWindow>
#include <QLabel>
#include <QPointF>

#include "SGlobals.h"
#include "SDrawingView.h"
#include "SSettingsWidget.h"

class SMainWnd : public QMainWindow
{
    Q_OBJECT

public:
    SMainWnd(QWidget *parent = 0);
    ~SMainWnd();

private:
    SDrawingView* mpDrawingView;
    SSettingsWidget* mpSettingsWidget;
};

#endif // SMAINWND_H

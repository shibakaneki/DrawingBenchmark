#include "smainwnd.h"

SMainWnd::SMainWnd(QWidget *parent):QMainWindow(parent)
  , mpDrawingView(NULL)
  , mpSettingsWidget(NULL)
{
    mpSettingsWidget = new SSettingsWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, mpSettingsWidget);

    mpDrawingView = new SDrawingView(this);
    setCentralWidget(mpDrawingView);
    connect(mpSettingsWidget, SIGNAL(smoothnessChanged(int)), mpDrawingView, SLOT(onSmoothnessChanged(int)));
    connect(mpDrawingView, SIGNAL(currentPointChanged(QPointF)), mpSettingsWidget, SLOT(onPosChanged(QPointF)));
}

SMainWnd::~SMainWnd()
{
    DELETEPTR(mpSettingsWidget);
    DELETEPTR(mpDrawingView);
}

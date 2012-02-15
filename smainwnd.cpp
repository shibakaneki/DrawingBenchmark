#include <QApplication>
#include "smainwnd.h"

SMainToolBar::SMainToolBar(QWidget *parent, const char *name):QToolBar(parent)
{
    setObjectName(name);
}

SMainToolBar::~SMainToolBar()
{

}

void SMainToolBar::enterEvent(QEvent *ev)
{
    Q_UNUSED(ev);
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
}

void SMainToolBar::leaveEvent(QEvent *ev)
{
    Q_UNUSED(ev);
    QApplication::restoreOverrideCursor();
}

// ---------------------------------------------------------------------------------------------------------
SMainWnd::SMainWnd(QWidget *parent):QMainWindow(parent)
  , mpColorWidget(NULL)
  , mpDrawingView(NULL)
  , mpSettingsWidget(NULL)
  , mpBrushPropertiesWidget(NULL)
  , mpToolBar(NULL)
  , mpClearAction(NULL)
  , mpArrowAction(NULL)
  , mpPenAction(NULL)
  , mpZoomInAction(NULL)
  , mpZoomOutAction(NULL)
{
    // DockWidgets
    mpSettingsWidget = new SSettingsWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, mpSettingsWidget);
    mpBrushPropertiesWidget = new SBrushPropertiesWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, mpBrushPropertiesWidget);
    mpColorWidget = new SColorWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, mpColorWidget);


    // Drawing Area
    mpDrawingView = new SDrawingView(this);
    setCentralWidget(mpDrawingView);

    // Toolbars
    mpToolBar = new SMainToolBar(this);
    mpToolBar->setWindowTitle(tr("General Toolbar"));
    addToolBar(mpToolBar);
    mpClearAction = mpToolBar->addAction(QIcon(":/res/clearPage.png"), tr("Clear the canvas"));
    mpToolBar->addSeparator();
    mpArrowAction = mpToolBar->addAction(QIcon(":/res/arrow.png"), tr("Select items"));
    mpArrowAction->setCheckable(true);
    mpPenAction = mpToolBar->addAction(QIcon(":/res/pen.png"), tr("Draw with the pen"));
    mpPenAction->setCheckable(true);
    mpEraserAction = mpToolBar->addAction(QIcon(":/res/eraser.png"), tr("Erase"));
    mpEraserAction->setCheckable(true);
    mpToolBar->addSeparator();
    mpPanAction = mpToolBar->addAction(QIcon(":/res/pan.png"), tr("pan"));
    mpPanAction->setCheckable(true);
    mpPanAction->setVisible(false);
    mpZoomInAction = mpToolBar->addAction(QIcon(":/res/zoomIn.png"), tr("Zoom In"));
    mpZoomInAction->setCheckable(true);
    mpZoomOutAction = mpToolBar->addAction(QIcon(":/res/zoomOut.png"), tr("Zoom Out"));
    mpZoomOutAction->setCheckable(true);
    mpZoomOutAction->setEnabled(false);
    onPenClicked();

    // Signal/Slots
    connect(mpSettingsWidget, SIGNAL(smoothnessChanged(int)), mpDrawingView, SLOT(onSmoothnessChanged(int)));
    connect(mpDrawingView, SIGNAL(currentPointChanged(QPointF)), mpSettingsWidget, SLOT(onPosChanged(QPointF)));
    connect(mpClearAction, SIGNAL(triggered()), mpDrawingView, SLOT(onClearPage()));
    connect(mpArrowAction, SIGNAL(triggered()), this, SLOT(onArrowClicked()));
    connect(mpPenAction, SIGNAL(triggered()), this, SLOT(onPenClicked()));
    connect(mpEraserAction, SIGNAL(triggered()), this, SLOT(onEraserClicked()));
    connect(mpZoomInAction, SIGNAL(triggered()), this, SLOT(onZoomInClicked()));
    connect(mpZoomOutAction, SIGNAL(triggered()), this, SLOT(onZoomOutClicked()));
    connect(mpPanAction, SIGNAL(triggered()), this, SLOT(onPanClicked()));
    connect(this, SIGNAL(currentToolChanged(eTool)), mpDrawingView, SLOT(onSetCurrentTool(eTool)));
    connect(mpDrawingView, SIGNAL(clearCoefficients()), mpSettingsWidget, SLOT(onClearCoefficients()));
    connect(mpDrawingView, SIGNAL(addCoefficients(QPointF,QPointF,QPointF,QPointF)), mpSettingsWidget, SLOT(onAddCoefficients(QPointF,QPointF,QPointF,QPointF)));
    connect(mpSettingsWidget, SIGNAL(pointSelected(QPointF,QPointF,QPointF,QPointF)), mpDrawingView, SLOT(onPointSelected(QPointF,QPointF,QPointF,QPointF)));
    connect(mpDrawingView, SIGNAL(zoomChanged(int)), this, SLOT(onZoomChanged(int)));
    connect(mpBrushPropertiesWidget, SIGNAL(lineWidthChanged(int)), mpDrawingView, SLOT(onLineWidthChanged(int)));
}

SMainWnd::~SMainWnd()
{
    DELETEPTR(mpPanAction);
    DELETEPTR(mpZoomOutAction);
    DELETEPTR(mpZoomInAction);
    DELETEPTR(mpPenAction);
    DELETEPTR(mpArrowAction);
    DELETEPTR(mpClearAction);
    DELETEPTR(mpToolBar);
    DELETEPTR(mpBrushPropertiesWidget);
    DELETEPTR(mpSettingsWidget);
    DELETEPTR(mpDrawingView);
    DELETEPTR(mpColorWidget);
}

void SMainWnd::onArrowClicked()
{
    QApplication::restoreOverrideCursor();
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    mpPanAction->setChecked(false);
    mpZoomInAction->setChecked(false);
    mpZoomOutAction->setChecked(false);
    mpPenAction->setChecked(false);
    mpEraserAction->setChecked(false);
    mpArrowAction->setChecked(true);
    emit currentToolChanged(eTool_Arrow);
}

void SMainWnd::onPenClicked()
{
    QApplication::restoreOverrideCursor();
    QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    mpPanAction->setChecked(false);
    mpZoomInAction->setChecked(false);
    mpZoomOutAction->setChecked(false);
    mpPenAction->setChecked(true);
    mpEraserAction->setChecked(false);
    mpArrowAction->setChecked(false);
    emit currentToolChanged(eTool_Pen);
}

void SMainWnd::onEraserClicked()
{
    QApplication::restoreOverrideCursor();
    QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    mpPanAction->setChecked(false);
    mpZoomInAction->setChecked(false);
    mpZoomOutAction->setChecked(false);
    mpPenAction->setChecked(false);
    mpEraserAction->setChecked(true);
    mpArrowAction->setChecked(false);
    emit currentToolChanged(eTool_Eraser);
}

void SMainWnd::onZoomInClicked()
{
    QApplication::restoreOverrideCursor();
    QApplication::setOverrideCursor(QCursor(QPixmap(":/res/zoomIn.png")));
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    mpPanAction->setChecked(false);
    mpZoomInAction->setChecked(true);
    mpZoomOutAction->setChecked(false);
    mpPenAction->setChecked(false);
    mpEraserAction->setChecked(false);
    mpArrowAction->setChecked(false);
    emit currentToolChanged(eTool_ZoomIn);
}

void SMainWnd::onZoomOutClicked()
{
    QApplication::restoreOverrideCursor();
    QApplication::setOverrideCursor(QCursor(QPixmap(":/res/zoomOut.png")));
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    mpPanAction->setChecked(false);
    mpZoomInAction->setChecked(false);
    mpZoomOutAction->setChecked(true);
    mpPenAction->setChecked(false);
    mpEraserAction->setChecked(false);
    mpArrowAction->setChecked(false);
    emit currentToolChanged(eTool_ZoomOut);
}

void SMainWnd::onPanClicked()
{
    QApplication::restoreOverrideCursor();
    QApplication::setOverrideCursor(QCursor(QPixmap(":/res/pan.png")));
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    mpPanAction->setChecked(true);
    mpZoomInAction->setChecked(false);
    mpZoomOutAction->setChecked(false);
    mpPenAction->setChecked(false);
    mpEraserAction->setChecked(false);
    mpArrowAction->setChecked(false);
    emit currentToolChanged(eTool_Pan);
}

void SMainWnd::onZoomChanged(int zoomDepth)
{
    if(0 == zoomDepth){
        mpZoomOutAction->setEnabled(false);
    }else{
        mpZoomOutAction->setEnabled(true);
    }
}

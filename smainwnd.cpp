#include <QApplication>
#include "smainwnd.h"

SMainWnd::SMainWnd(QWidget *parent):QMainWindow(parent)
  , mpDrawingView(NULL)
  , mpSettingsWidget(NULL)
  , mpToolBar(NULL)
  , mpClearAction(NULL)
  , mpArrowAction(NULL)
  , mpPenAction(NULL)
{
    // DockWidgets
    mpSettingsWidget = new SSettingsWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, mpSettingsWidget);

    // Drawing Area
    mpDrawingView = new SDrawingView(this);
    setCentralWidget(mpDrawingView);

    // Toolbars
    mpToolBar = new QToolBar(tr("General"), this);
    addToolBar(mpToolBar);
    mpClearAction = mpToolBar->addAction(QIcon(":/res/clearPage.png"), tr("Clear the canvas"));
    mpToolBar->addSeparator();
    mpArrowAction = mpToolBar->addAction(QIcon(":/res/arrow.png"), tr("Select items"));
    mpArrowAction->setCheckable(true);
    mpPenAction = mpToolBar->addAction(QIcon(":/res/pen.png"), tr("Draw with the pen"));
    mpPenAction->setCheckable(true);
    mpEraserAction = mpToolBar->addAction(QIcon(":/res/eraser.png"), tr("Erase"));
    mpEraserAction->setCheckable(true);
    onPenClicked();

    // Signal/Slots
    connect(mpSettingsWidget, SIGNAL(smoothnessChanged(int)), mpDrawingView, SLOT(onSmoothnessChanged(int)));
    connect(mpDrawingView, SIGNAL(currentPointChanged(QPointF)), mpSettingsWidget, SLOT(onPosChanged(QPointF)));
    connect(mpClearAction, SIGNAL(triggered()), mpDrawingView, SLOT(onClearPage()));
    connect(mpArrowAction, SIGNAL(triggered()), this, SLOT(onArrowClicked()));
    connect(mpPenAction, SIGNAL(triggered()), this, SLOT(onPenClicked()));
    connect(mpEraserAction, SIGNAL(triggered()), this, SLOT(onEraserClicked()));
    connect(this, SIGNAL(currentToolChanged(eTool)), mpDrawingView, SLOT(onSetCurrentTool(eTool)));
}

SMainWnd::~SMainWnd()
{
    DELETEPTR(mpPenAction);
    DELETEPTR(mpArrowAction);
    DELETEPTR(mpClearAction);
    DELETEPTR(mpToolBar);
    DELETEPTR(mpSettingsWidget);
    DELETEPTR(mpDrawingView);
}

void SMainWnd::onArrowClicked()
{
    QApplication::restoreOverrideCursor();
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    mpPenAction->setChecked(false);
    mpEraserAction->setChecked(false);
    mpArrowAction->setChecked(true);
    emit currentToolChanged(eTool_Arrow);
}

void SMainWnd::onPenClicked()
{
    QApplication::restoreOverrideCursor();
    QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
    mpPenAction->setChecked(true);
    mpEraserAction->setChecked(false);
    mpArrowAction->setChecked(false);
    emit currentToolChanged(eTool_Pen);
}

void SMainWnd::onEraserClicked()
{
    QApplication::restoreOverrideCursor();
    QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
    mpPenAction->setChecked(false);
    mpEraserAction->setChecked(true);
    mpArrowAction->setChecked(false);
    emit currentToolChanged(eTool_Eraser);
}

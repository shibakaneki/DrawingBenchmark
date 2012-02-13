#ifndef SMAINWND_H
#define SMAINWND_H

#include <QMainWindow>
#include <QLabel>
#include <QPointF>
#include <QToolBar>
#include <QAction>

#include "SGlobals.h"
#include "SDrawingView.h"
#include "SSettingsWidget.h"
#include "SColorWidget.h"

class SMainWnd : public QMainWindow
{
    Q_OBJECT

public:
    SMainWnd(QWidget *parent = 0);
    ~SMainWnd();

signals:
    void currentToolChanged(eTool tool);

private slots:
    void onArrowClicked();
    void onPenClicked();
    void onEraserClicked();

private:
    SColorWidget* mpColorWidget;
    SDrawingView* mpDrawingView;
    SSettingsWidget* mpSettingsWidget;
    QToolBar* mpToolBar;
    QAction* mpClearAction;
    QAction* mpArrowAction;
    QAction* mpPenAction;
    QAction* mpEraserAction;
};

#endif // SMAINWND_H

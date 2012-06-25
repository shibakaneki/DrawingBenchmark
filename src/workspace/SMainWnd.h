#ifndef SMAINWND_H
#define SMAINWND_H

#include <QMainWindow>
#include <QLabel>
#include <QPointF>
#include <QToolBar>
#include <QAction>
#include <QStackedWidget>

#include "core/SGlobals.h"
#include "workspace/SDrawingView.h"
#include "workspace/SLeafWidget.h"
#include "dockpalettes/SColorWidget.h"
#include "dockpalettes/SBrushPropertiesWidget.h"
#include "dockpalettes/SLayerPalette.h"
#include "graphicsitems/SGraphicsCanvasItem.h"

class SMainToolBar : public QToolBar
{
    Q_OBJECT
public:
    SMainToolBar(QWidget* parent=0, const char* name="SMainToolBar");
    virtual ~SMainToolBar();

protected:
    void enterEvent(QEvent* ev);
    void leaveEvent(QEvent* ev);
};

class SMainWnd : public QMainWindow
{
    Q_OBJECT

public:
    SMainWnd(QWidget *parent = 0);
    virtual ~SMainWnd();

signals:
    void currentToolChanged(eTool tool);

private slots:
    void onArrowClicked();
    void onPenClicked();
    void onEraserClicked();
    void onZoomInClicked();
    void onZoomOutClicked();
    void onPanClicked();
    void onZoomChanged(int zoomDepth);

private:
    QStackedWidget* mpStack;
    SBrushPropertiesWidget* mpBrushPropertiesWidget;
    SColorWidget* mpColorWidget;
    SDrawingView* mpDrawingView;
    SMainToolBar* mpToolBar;
    QAction* mpClearAction;
    QAction* mpArrowAction;
    QAction* mpPenAction;
    QAction* mpEraserAction;
    QAction* mpZoomInAction;
    QAction* mpZoomOutAction;
    QAction* mpPanAction;
    SLeafWidget* mpLeaf;
    SLayerPalette* mpLayers;
    SGraphicsCanvasItem* mpCanvas;
};

#endif // SMAINWND_H

#ifndef SGRAPHICSCANVASITEM_H
#define SGRAPHICSCANVASITEM_H

#include <QGraphicsProxyWidget>
#include <QGraphicsSceneResizeEvent>
#include <QGraphicsSceneMouseEvent>
#include <QTabletEvent>

#include "core/SGlobals.h"
#include "workspace/SDrawingView.h"

class SGraphicsCanvasItem : public QGraphicsProxyWidget{
public:
    SGraphicsCanvasItem();
    ~SGraphicsCanvasItem();

    void resizeCanvas(int w, int h);
    void addLayer(SDrawingView* pLayer);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* ev);

private:
    QList<SDrawingView*> mLayers;
    QWidget* mpContainer;

};

#endif // SGRAPHICSCANVASITEM_H

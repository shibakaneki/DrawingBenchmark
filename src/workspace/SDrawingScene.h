#ifndef SDRAWINGSCENE_H
#define SDRAWINGSCENE_H

#include <QGraphicsScene>
#include "core/SGlobals.h"

class SDrawingScene : public QGraphicsScene
{
public:
    SDrawingScene(QObject* parent=0);
    ~SDrawingScene();
};

#endif // SDRAWINGSCENE_H

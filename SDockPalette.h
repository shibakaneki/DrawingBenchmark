#ifndef SDOCKPALETTE_H
#define SDOCKPALETTE_H

#include <QDockWidget>
#include "SGlobals.h"

class SDockPalette : public QDockWidget
{
    Q_OBJECT
public:
    SDockPalette(QWidget* parent=0, const char* name="SDockPalette");
    ~SDockPalette();
};

#endif // SDOCKPALETTE_H

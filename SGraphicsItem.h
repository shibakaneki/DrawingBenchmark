#ifndef SGRAPHICSITEM_H
#define SGRAPHICSITEM_H

#include "SSelectionRect.h"
#include "SGlobals.h"

class SGraphicsItem
{
public:
    SGraphicsItem();
    ~SGraphicsItem();
    SSelectionRect* selectionRect(){return mpSelectionRect;}

protected:
        SSelectionRect* mpSelectionRect;
};

#endif // SGRAPHICSITEM_H

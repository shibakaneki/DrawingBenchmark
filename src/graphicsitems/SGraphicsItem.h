#ifndef SGRAPHICSITEM_H
#define SGRAPHICSITEM_H

#include "workspace/SSelectionRect.h"
#include "core/SGlobals.h"

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

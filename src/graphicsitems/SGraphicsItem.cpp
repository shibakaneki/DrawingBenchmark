#include "SGraphicsItem.h"

SGraphicsItem::SGraphicsItem():mpSelectionRect(NULL)
{

}


SGraphicsItem::~SGraphicsItem()
{
    DELETEPTR(mpSelectionRect);
}

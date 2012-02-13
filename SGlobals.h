#ifndef SGLOBALS_H
#define SGLOBALS_H

#define DELETEPTR(ptr) if(NULL != ptr){ \
    delete ptr; \
    ptr = NULL; \
    }

typedef enum{
    eTool_Arrow,
    eTool_Pen,
    eTool_Eraser,
    eTool_ZoomIn,
    eTool_ZoomOut,
    eTool_Pan
}eTool;

#endif // SGLOBALS_H

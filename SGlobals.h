#ifndef SGLOBALS_H
#define SGLOBALS_H

#define DELETEPTR(ptr) if(NULL != ptr){ \
    delete ptr; \
    ptr = NULL; \
    }

#endif // SGLOBALS_H

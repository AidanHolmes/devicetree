#ifndef __DEVTREELIBRARY_H
#define __DEVTREELIBRARY_H

#include <exec/types.h>
#include "libdev.h"


struct LibDevBase* __saveds __asm libdev_library_open(register __a6 struct LibDevBase *base);
struct LibDevBase* __saveds __asm libdev_initalise(register __a6 struct LibDevBase *base) ;
void __saveds __asm libdev_cleanup(register __a6 struct LibDevBase *base);


#endif
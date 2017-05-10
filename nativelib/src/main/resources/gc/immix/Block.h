#ifndef IMMIX_BLOCK_H
#define IMMIX_BLOCK_H

#include "headers/BlockHeader.h"
#include "Heap.h"
#include "Line.h"

#define LAST_HOLE -1


void block_recycle(Allocator*, BlockHeader*);

#endif //IMMIX_BLOCK_H
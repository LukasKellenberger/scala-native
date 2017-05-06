#ifndef IMMIX_BLOCK_H
#define IMMIX_BLOCK_H

#include "headers/BlockHeader.h"
#include "Heap.h"
#include "Line.h"

#define LAST_HOLE -1

bool block_lineContainsObject(BlockHeader*, int);
ObjectHeader* block_lineGetFirstObject(BlockHeader*, int);

BlockHeader* block_getNextBlock(BlockHeader*);

void block_recycle(Allocator*, BlockHeader*);

void block_print(BlockHeader*);

#endif //IMMIX_BLOCK_H

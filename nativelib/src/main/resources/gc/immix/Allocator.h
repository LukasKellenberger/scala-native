//
// Created by Lukas Kellenberger on 20.04.17.
//

#ifndef IMMIX_ALLOCATOR_H
#define IMMIX_ALLOCATOR_H

#include "headers/gc_types.h"
#include "BlockList/BlocList.h"

typedef struct {
    word_t* heapStart;
    BlockList recycledBlocks;
    BlockList freeBlocks;
    word_t* cursor;
    word_t* limit;
    word_t* largeCursor;
    word_t* largeLimit;
    int freeBlockCount;
    int recyclableBlockCount;
} Allocator;


Allocator* allocator_create(word_t*, int);
bool allocator_initCursors(Allocator* allocator);
word_t* allocator_alloc(Allocator* allocator, size_t size);

#endif //IMMIX_ALLOCATOR_H

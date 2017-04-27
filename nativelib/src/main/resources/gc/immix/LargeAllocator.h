//
// Created by Lukas Kellenberger on 24.04.17.
//

#ifndef IMMIX_LARGEALLOCATOR_H
#define IMMIX_LARGEALLOCATOR_H

#include "bitmap/Bitmap.h"
#include "headers/gc_types.h"
#include "Constants.h"
#include "headers/ObjectHeader.h"

#define FREE_LIST_COUNT (LARGE_OBJECT_MAX_SIZE_BITS - LARGE_OBJECT_MIN_SIZE_BITS + 1)


typedef struct Chunk Chunk;

struct Chunk {
    ObjectHeaderLine header;
    Chunk* next;
};

typedef struct {
    Chunk* first;
    Chunk* last;
} FreeList;


typedef struct {
    word_t* offset;
    size_t size;
    FreeList freeLists[FREE_LIST_COUNT];
    Bitmap* bitmap;
} LargeAllocator;

LargeAllocator* largeAllocator_create(word_t*, size_t);
ObjectHeader* largeAllocator_getBlock(LargeAllocator* allocator, size_t requestedBlockSize);
void largeAllocator_sweep(LargeAllocator* allocator);

#endif //IMMIX_LARGEALLOCATOR_H

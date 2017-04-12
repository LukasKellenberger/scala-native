#ifndef IMMIX_IMMIX_ALLOCATOR_H
#define IMMIX_IMMIX_ALLOCATOR_H

#include <stdlib.h>
#include "Immix_space.h"

typedef struct {
    ImmixSpace* space;
    bool hot;
    bool copy;
    void* cursor;
    void* limit;
    void* largeCursor;
    void* largeLimit;
    bool requestForLarge;
    int lineUseCount;
    void* markTable;
    void* recyclableBlock;
    int line;
    bool recyclableExhausted;
} ImmixAllocator;

ImmixAllocator* immix_allocator_constructor(ImmixSpace* space, bool hot, bool copy);

void immix_allocator_reset(ImmixAllocator* immixAllocator);

void* immix_allocator_alloc(ImmixAllocator* immixAllocator, int bytes, int align, int offset);

void* immix_allocator_overflow_alloc(ImmixAllocator* immixAllocator, int bytes, int align, int offset);


#endif //IMMIX_IMMIX_ALLOCATOR_H
#ifndef IMMIX_ALLOCATOR_H
#define IMMIX_ALLOCATOR_H

#include "GCTypes.h"
#include <stddef.h>
#include "datastructures/BlocList.h"
#include "stats/AllocatorStats.h"

typedef struct {
    word_t* heapStart;
    uint64_t blockCount;
    BlockList recycledBlocks;
    uint64_t recycledBlockCount;
    BlockList freeBlocks;
    uint64_t freeBlockCount;
    BlockHeader* block;
    word_t* cursor;
    word_t* limit;
    BlockHeader* largeBlock;
    word_t* largeCursor;
    word_t* largeLimit;
#ifdef ALLOCATOR_STATS
    AllocatorStats* stats;
#endif
} Allocator;


Allocator* allocator_create(word_t*, int);
bool allocator_canInitCursors(Allocator* allocator);
void allocator_initCursors(Allocator* allocator);
word_t* allocator_alloc(Allocator* allocator, size_t size);

bool allocator_shouldGrow(Allocator* allocator);

#endif //IMMIX_ALLOCATOR_H

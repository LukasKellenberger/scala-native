#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <time.h>
#include "Heap.h"
#include "Block.h"
#include "Log.h"
#include "Allocator.h"
#include "stats/AllocatorStats.h"
#include "Marker.h"
#include "State.h"
#include "headers/BlockHeader.h"
#include "utils/MathUtils.h"
#include "LargeAllocator.h"

#define MAX_SIZE 64*1024*1024*1024L
// Allow read and write
#define HEAP_MEM_PROT (PROT_READ | PROT_WRITE)
// Map private anonymous memory, and prevent from reserving swap
#define HEAP_MEM_FLAGS (MAP_NORESERVE | MAP_PRIVATE | MAP_ANONYMOUS)
// Map anonymous memory (not a file)
#define HEAP_MEM_FD -1
#define HEAP_MEM_FD_OFFSET 0


word_t* mapAndAlign(int alignmentMask) {
    word_t* heapStart = mmap(NULL, MAX_SIZE, HEAP_MEM_PROT, HEAP_MEM_FLAGS, HEAP_MEM_FD, HEAP_MEM_FD_OFFSET);

    // Heap start not aligned on
    if(((word_t)heapStart & alignmentMask) != (word_t)heapStart) {
        word_t* previousBlock = (word_t*)((word_t)heapStart & BLOCK_SIZE_IN_BYTES_INVERSE_MASK);
        heapStart = previousBlock + WORDS_IN_BLOCK;
    }
    return heapStart;
}

Heap* heap_create(size_t initialSize) {
    assert(initialSize >= 2*BLOCK_TOTAL_SIZE);
    assert(initialSize % BLOCK_TOTAL_SIZE == 0);

    Heap* heap = malloc(sizeof(Heap));

    word_t* smallHeapStart = mapAndAlign(BLOCK_SIZE_IN_BYTES_INVERSE_MASK);


    heap->smallHeapSize = initialSize;
    heap->heapStart = smallHeapStart;
    heap->heapEnd = smallHeapStart + initialSize / sizeof(word_t);
    heap->allocator = allocator_create(smallHeapStart, initialSize / BLOCK_TOTAL_SIZE);

    initialSize = 256 * 1024 * 1024;

    word_t* largeHeapStart = mapAndAlign(LARGE_BLOCK_MASK);
    heap->largeHeapSize = initialSize;
    heap->largeAllocator = largeAllocator_create(largeHeapStart, initialSize);
    heap->largeHeapStart = largeHeapStart;
    heap->largeHeapEnd = (word_t*)((ubyte_t*)largeHeapStart + initialSize);


    return heap;
}


word_t* heap_allocLarge(Heap* heap, uint32_t objectSize) {
    assert(objectSize % 8 == 0);

    uint32_t size = objectSize + sizeof(word_t); // Add header
    ObjectHeader* object = largeAllocator_getBlock(heap->largeAllocator, size);
    if(object != NULL) {
        object_setObjectType(object, object_large);
        object_setSize(object, size);
        return (word_t*) object + 1;
    } else {
        heap_collect(heap, stack);

        object = largeAllocator_getBlock(heap->largeAllocator, size);
        if(object != NULL) {
            object_setObjectType(object, object_large);
            object_setSize(object, size);
            return (word_t*) object + 1;
        } else {
            //heap_growLarge(heap, size);
            //return heap_allocLarge(heap, objectSize);
            largeAllocator_print(heap->largeAllocator);
            printf("Failed to alloc: %u\n", size + 8);
            printf("No more memory available\n");
            fflush(stdout);
            exit(1);
        }
    }

}

word_t* allocSmallSlow(Heap* heap, uint32_t size) {

    heap_collect(heap, stack);

    ObjectHeader *block = (ObjectHeader *) allocator_alloc(heap->allocator, size);
    if (block != NULL) {
        object_setObjectType(block, object_standard);
        object_setSize(block, size);

#ifdef ALLOCATOR_STATS
        heap->allocator->stats->bytesAllocated += objectSize;
            heap->allocator->stats->totalBytesAllocated += objectSize;
            heap->allocator->stats->totalAllocatedObjectCount++;
#endif
    }

    if(block == NULL) {
        largeAllocator_print(heap->largeAllocator);
        printf("Failed to alloc: %u\n", size + 8);
        printf("No more memory available\n");
        fflush(stdout);
        exit(1);
    }

    return (word_t*) block + 1;
}


INLINE word_t* heap_allocSmall(Heap* heap, uint32_t objectSize) {
    assert(objectSize % 8 == 0);
    uint32_t size = objectSize + sizeof(word_t); // Add header
    ObjectHeader *block = (ObjectHeader *) allocator_alloc(heap->allocator, size);
    if (block != NULL) {
        object_setObjectType(block, object_standard);
        object_setSize(block, size);

#ifdef ALLOCATOR_STATS
        heap->allocator->stats->bytesAllocated += objectSize;
            heap->allocator->stats->totalBytesAllocated += objectSize;
            heap->allocator->stats->totalAllocatedObjectCount++;
#endif
        return (word_t*)block + 1;
    } else {
        return allocSmallSlow(heap, size);
    }
}

word_t* heap_alloc(Heap* heap, uint32_t objectSize) {
    assert(objectSize % 8 == 0);

    if(objectSize + sizeof(word_t) >= LARGE_BLOCK_SIZE) {
        return heap_allocLarge(heap, objectSize);
    } else {
        return heap_allocSmall(heap, objectSize);
    }
}

void heap_collect(Heap* heap, Stack* stack) {
#if defined(DEBUG_PRINT) || defined(TIMING_PRINT)
    printf("\nCollect\n");
    fflush(stdout);
#endif
#ifdef TIMING_PRINT
    clock_t start = clock(), diff;
#endif
    mark_roots(heap, stack);

#ifdef TIMING_PRINT
    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);

    start = clock();
#endif

    heap_recycle(heap);

#ifdef TIMING_PRINT
    diff = clock() - start;
    msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);
#endif
#if defined(DEBUG_PRINT) || defined(TIMING_PRINT)
    printf("End collect\n");
    fflush(stdout);
#endif
}

void heap_recycle(Heap* heap) {
    blockList_clear(&heap->allocator->recycledBlocks);
    blockList_clear(&heap->allocator->freeBlocks);
    heap->allocator->freeBlockCount = 0;
    heap->allocator->recycledBlockCount = 0;

#ifdef ALLOCATOR_STATS
    allocatorStats_resetBlockDistribution(heap->allocator->stats);
#endif

    word_t* current = heap->heapStart;
    while(current != heap->heapEnd) {
        BlockHeader* blockHeader = (BlockHeader*) current;
        block_recycle(heap->allocator, blockHeader);
        //block_print(blockHeader);
        current += WORDS_IN_BLOCK;
    }
    largeAllocator_sweep(heap->largeAllocator);

#ifdef ALLOCATOR_STATS
    allocatorStats_print(heap->allocator->stats);
    heap->allocator->stats->liveObjectCount = 0;
    heap->allocator->stats->bytesAllocated = 0;
#endif
    if(!allocator_canInitCursors(heap->allocator)) {
        exit(1);
    }
    /*if(!allocator_canInitCursors(heap->allocator) || allocator_shouldGrow(heap->allocator)) {
        size_t increment = heap->smallHeapSize / WORD_SIZE * GROWTH_RATE / 100;
        increment = (increment - 1 + WORDS_IN_BLOCK) / WORDS_IN_BLOCK * WORDS_IN_BLOCK;
        heap_grow(heap, increment);
    }*/
    allocator_initCursors(heap->allocator);
}

// increment in words
void heap_grow(Heap* heap, size_t increment) {
    assert(increment % WORDS_IN_BLOCK == 0);

#ifdef DEBUG_PRINT
    printf("Growing small heap by %zu bytes, to %zu bytes\n", increment * WORD_SIZE, heap->smallHeapSize + increment * WORD_SIZE);
    fflush(stdout);
#endif

    word_t* heapEnd = heap->heapEnd;
    heap->heapEnd = heapEnd + increment;
    heap->smallHeapSize += increment * WORD_SIZE;


    BlockHeader* lastBlock = (BlockHeader*)(heap->heapEnd - WORDS_IN_BLOCK);
    blockList_addBlocksLast(&heap->allocator->freeBlocks, (BlockHeader*)heapEnd, lastBlock);

    heap->allocator->blockCount += increment / WORDS_IN_BLOCK;
    heap->allocator->freeBlockCount += increment / WORDS_IN_BLOCK;
}

void heap_growLarge(Heap* heap, size_t increment) {
    increment = 1UL << log2_ceil(increment);

#ifdef DEBUG_PRINT
    printf("Growing large heap by %zu bytes, to %zu bytes\n", increment * WORD_SIZE, heap->largeHeapSize + increment * WORD_SIZE);
    fflush(stdout);
#endif

    word_t* heapEnd = heap->largeHeapEnd;
    heap->largeHeapEnd += increment;
    heap->largeHeapSize += increment * WORD_SIZE;
    heap->largeAllocator->size += increment * WORD_SIZE;

    bitmap_grow(heap->largeAllocator->bitmap, increment * WORD_SIZE);

    largeAllocator_addChunk(heap->largeAllocator, (Chunk*)heapEnd, increment * WORD_SIZE);
}
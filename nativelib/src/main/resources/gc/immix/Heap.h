//
// Created by Lukas Kellenberger on 20.04.17.
//

#ifndef IMMIX_HEAP_H
#define IMMIX_HEAP_H

#include "headers/gc_types.h"
#include "Allocator.h"
#include "LargeAllocator.h"

typedef struct {
    word_t* heapStart;
    word_t* heapEnd;
    size_t smallHeapSize;
    word_t* largeHeapStart;
    word_t* largeHeapEnd;
    word_t* largeHeapSize;
    Allocator* allocator;
    LargeAllocator* largeAllocator;
} Heap;

Heap* heap_create(size_t);
ObjectHeader* heap_alloc(Heap*, uint32_t);
bool heap_isWordInHeap(Heap*, word_t*);
bool heap_isWordInSmallHeap(Heap* heap, word_t* word);
bool heap_isWordInLargeHeap(Heap* heap, word_t* word);
bool heap_isObjectInHeap(Heap*, ObjectHeader*);
bool heap_recycle(Heap*);
void heap_grow(Heap*, size_t);

#endif //IMMIX_HEAP_H

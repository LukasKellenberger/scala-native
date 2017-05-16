#ifndef IMMIX_HEAP_H
#define IMMIX_HEAP_H

#include "GCTypes.h"
#include "Allocator.h"
#include "LargeAllocator.h"
#include "datastructures/Stack.h"

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

static INLINE bool heap_isWordInLargeHeap(Heap* heap, word_t* word) {
    return word != NULL && word >= heap->largeHeapStart && word < heap->largeHeapEnd;
}
static INLINE bool heap_isWordInSmallHeap(Heap* heap, word_t* word) {
    return word != NULL && word >= heap->heapStart && word < heap->heapEnd;
}

static INLINE bool heap_isWordInHeap(Heap* heap, word_t* word) {
    return heap_isWordInSmallHeap(heap, word) || heap_isWordInLargeHeap(heap, word);
}
static INLINE bool heap_isObjectInHeap(Heap* heap, ObjectHeader* object) {
    return heap_isWordInHeap(heap, (word_t*) object);
}

static INLINE bool heap_isInObjectInHeapFast(word_t* heapStart, word_t* heapEnd, word_t* largeHeapStart, word_t* largeHeapEnd, ObjectHeader* objectHeader) {
    word_t* word = (word_t*) objectHeader;
    return (word >= heapStart && word < heapEnd) || (word >= largeHeapStart && word < largeHeapEnd);
}

Heap* heap_create(size_t initialHeapSize);
word_t* heap_alloc(Heap* heap, uint32_t objectSize);
word_t* heap_allocSmall(Heap* heap, uint32_t objectSize);
word_t* heap_allocLarge(Heap* heap, uint32_t objectSize);

void heap_collect(Heap* heap, Stack* stack);

bool heap_recycle(Heap* heap);
void heap_grow(Heap*, size_t);

#endif //IMMIX_HEAP_H

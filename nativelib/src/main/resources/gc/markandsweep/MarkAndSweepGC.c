#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <memory.h>
#include "Heap.h"
#include "Constants.h"
#include "headers/ObjectHeader.h"
#include "Marker.h"

Heap* heap = NULL;
Stack* stack = NULL;

void scalanative_init() {
    heap = heap_create();
    stack = stack_alloc(INITIAL_STACK_SIZE);
}

//bool a = false;

void scalanative_collect() {
    printf("Collect\n");
    fflush(stdout);

    mark_roots(heap, stack);
    heap_collect(heap);

    printf("End collect\n");
    fflush(stdout);
}

void* scalanative_alloc(void* info, size_t byteSize) {
    assert(heap != NULL);

    // Divide size by WORD_SIZE, round up and add 1 (for the header) to get the number of words
    uint32_t wordSize = (uint32_t)((byteSize + WORD_SIZE - 1) / WORD_SIZE) + 1;
    word_t* alloc = (word_t*)heap_alloc(heap, wordSize);
    if(alloc == NULL) {
        scalanative_collect();

        alloc = (word_t*)heap_alloc(heap, wordSize);
        if(alloc == NULL) {
            printf("Failed to alloc: %zu\n", byteSize);
            printf("Out of memory\n");
            fflush(stdout);
            exit(1);
        }
    }

    assert(heap_isWordInHeap(heap, alloc));

    memset(alloc + 1, 0, (wordSize - 1) * WORD_SIZE);
    void** allocc = (void**)(alloc + 1);
    *allocc = info;
    return (void*)allocc;
}

void* scalanative_alloc_small(void* info, size_t size) {
    return scalanative_alloc(info, size);
}

void* scalanative_alloc_large(void* info, size_t size) {
    return scalanative_alloc(info, size);
}

void* scalanative_alloc_atomic(void* info, size_t size) {
    return scalanative_alloc(info, size);
}



//
// Created by Lukas Kellenberger on 20.04.17.
//


#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "headers/gc_types.h"
#include "Heap.h"
#include "ProgramRoots.h"
#include "Stack.h"
#include "Marker.h"



#define INITIAL_HEAP_SIZE (256*1024*1024)


Heap* heap = NULL;
Roots* roots = NULL;
Stack* stack = NULL;


void scalanative_collect();

void scalanative_init() {
    heap = heap_create(INITIAL_HEAP_SIZE);
    stack = stack_alloc(INITIAL_STACK_SIZE);
}

void* scalanative_alloc_raw(size_t size) {
    assert(size <= MAX_BLOCK_SIZE);
    size = (size + sizeof(word_t) - 1 ) / sizeof(word_t) * sizeof(word_t);
    if(heap == NULL) {
        scalanative_init();
    }


    ObjectHeader* block = heap_alloc(heap, (uint32_t)size);
    if(block == NULL) {
        scalanative_collect();

        block = heap_alloc(heap, (uint32_t)size);
        if(block == NULL) {
            printf("No more memory available\n");
            fflush(stdout);
            exit(1);
        }

    }
    //printf("alloc: %p %zu\n", block, size + sizeof(word_t));
    //fflush(stdout);
    memset((word_t*)block + 1, 0, size);
    return (word_t*)block + 1;
}

void* scalanative_alloc_raw_atomic(size_t size) {
    return scalanative_alloc_raw(size);
}

void* scalanative_alloc(void* info, size_t size) {
    void** alloc = (void**) scalanative_alloc_raw(size);
    *alloc = info;
    //printf("alloc id: %d\n",((Rtti*)info)->id);
    return (void*) alloc;
}

void* alloc(size_t size) {
    return scalanative_alloc_raw(size);
}

void scalanative_collect() {
    printf("Collect\n");
    //printf("allocator: %p - %p, %p - %p\n", heap->allocator->cursor, heap->allocator->limit, heap->allocator->largeCursor, heap->allocator->largeLimit);
    fflush(stdout);
    //ProgramRoots_getRoots(&roots);
    //marker_markRoots(heap, roots, stack);
    //marker_mark(heap, stack);
    mark_roots(heap, stack);
    bool success = heap_recycle(heap);

    if(!success) {
        printf("No more memory available\n");
        fflush(stdout);
        exit(1);
    }
    //printf("allocator: %p - %p, %p - %p\n", heap->allocator->cursor, heap->allocator->limit, heap->allocator->largeCursor, heap->allocator->largeLimit);
    printf("End collect\n");
    fflush(stdout);
}

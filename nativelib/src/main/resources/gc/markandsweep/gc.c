//
// Created by Lukas Kellenberger on 01.03.17.
//

#include "gc.h"
#include "free_list.h"
#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "linked_list.h"
#include "mark.h"
#include "utils/free_list_utils.h"
#include <time.h>

#define UNW_LOCAL_ONLY

#include <libunwind.h>
#include <setjmp.h>

FreeList* free_list = NULL;
Heap* heap_ = NULL;

#define CHUNK 1024*1024*1024


void sweep() {
    word_t* current = heap_->heap_start;
    size_t current_size = 0;

    free_list_clear_lists(free_list);

    while(current != NULL) {
        current_size = header_unpack_size(current);
        // Current block is alive, set bit to 1 and go to next block
        if(!bitmap_get_bit(heap_->bitmap, current)) {
            bitmap_set_bit(heap_->bitmap, current);
            current = heap_next_block(heap_, current);

        } else {
            // Block is not alive, merge with next while possible
            word_t* next = heap_next_block(heap_, current);
            while(next != NULL && bitmap_get_bit(heap_->bitmap, next)) {
                current_size = current_size + header_unpack_size(next);
                bitmap_clear_bit(heap_->bitmap, next);
                next = heap_next_block(heap_, next);
            }
            free_list_add_block(free_list, current, current_size);
            current = next;
        }
    }
}

void scalanative_init() {
    heap_ = heap_alloc(CHUNK);
    free_list = heap_->free_list;
}


void* scalanative_alloc_raw(size_t size) {
    size = (size + sizeof(word_t) - 1 ) / sizeof(word_t) * sizeof(word_t);
    size_t nb_words = size / sizeof(word_t);
    if(free_list == NULL) {
        scalanative_init();
    }
    word_t* block = free_list_get_block(free_list, nb_words + 1);
    //free_list_print(free_list);
    assert(block == NULL || header_unpack_size(block) >= size/sizeof(word_t) + 1 && header_unpack_size(block) <= size/sizeof(word_t) + 2);
    assert(block == NULL || header_unpack_tag(block) == tag_allocated);
    if(block == NULL) {
        scalanative_collect();
        block = free_list_get_block(free_list, nb_words + 1);

        assert(block == NULL || header_unpack_size(block) > size/sizeof(word_t) + 1 && header_unpack_size(block) <= size/sizeof(word_t) + 2);
        assert(block == NULL || header_unpack_tag(block) == tag_allocated);

        if(block == NULL) {
            free_list_print_stats(free_list);

            printf("size: %zu\n", size);
            printf("No more memory available\n");
            fflush(stdout);
            exit(1);
        }
        memset(block + 1, 0, size);
        return block + 1;
    }

    assert(block + header_unpack_size(block) - 1 < free_list->start + free_list->size / sizeof(word_t));
    memset(block+1, 0, size);

    return block + 1;
}

void* scalanative_alloc_raw_atomic(size_t size) {
    return scalanative_alloc_raw(size);
}

void* scalanative_alloc(void* info, size_t size) {
    void** alloc = (void**) scalanative_alloc_raw(size);
    *alloc = info;
    return (void*) alloc;
}

void* alloc(size_t size) {
    return scalanative_alloc_raw(size);
}

void scalanative_collect() {

    #ifdef TIMING_PRINT
        printf("\n\n### START GC ###\n");
        fflush(stdout);

        clock_t start = clock(), diff;
    #endif

    mark_roots(heap_);

    #ifdef TIMING_PRINT
        diff = clock() - start;
        int msec = diff * 1000 / CLOCKS_PER_SEC;
        printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);

        start = clock();
    #endif

    sweep();

    #ifdef TIMING_PRINT
        diff = clock() - start;
        msec = diff * 1000 / CLOCKS_PER_SEC;
        printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);

        printf("### END GC ###\n");
        fflush(stdout);
    #endif
}
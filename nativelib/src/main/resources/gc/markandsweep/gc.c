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
#include "free_list_stats.h"
#include <time.h>

#define UNW_LOCAL_ONLY

#include <libunwind.h>
#include <setjmp.h>

FreeList* free_list = NULL;

#define CHUNK 768*1024*1024

#define DEBUG

void mark(word_t* block);


unsigned long long alloc_count;
unsigned long long alloc_size;
int gced = 0;

void memory_check(int print);

void sweep() {
    word_t* current = free_list->start;
    size_t current_size_with_header = 0;
    word_t* growing = NULL;
    size_t growing_size_with_header = 0;

    free_list_clear_lists(free_list);
    word_t* end = free_list->start + (free_list->size / sizeof(word_t));

    while(current < end) {
        current_size_with_header = header_unpack_size(current) + 1;
        if(!bitmap_get_bit(free_list->bitmap, current)) {
            bitmap_set_bit(free_list->bitmap, current);
            if(growing != NULL) {
                free_list_add_block(free_list, growing, growing_size_with_header - 1);
                growing = NULL;
            }
        } else {
            if(growing == NULL) {
                growing = current;
                growing_size_with_header = current_size_with_header;
            } else {
                growing_size_with_header += current_size_with_header;
                bitmap_clear_bit(free_list->bitmap, current);
            }

        }
        current += current_size_with_header;
    }
    if(growing != NULL) {
        free_list_add_block(free_list, growing, growing_size_with_header - 1);
    }
}

void scalanative_init() {
    free_list = free_list_create(CHUNK);
    LinkedList* linkedList = free_list->list[0];
    mark_init(free_list);
}


void* scalanative_alloc_raw(size_t size) {
    size = (size + 8 - 1 ) / 8 * 8;
    if(free_list == NULL) {
        scalanative_init();
    }
    word_t* block = free_list_get_block(free_list, size);
    assert(block == NULL || header_unpack_size(block) >= size/sizeof(word_t));

    if(block == NULL) {
        scalanative_collect();
        block = free_list_get_block(free_list, size);
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

    assert(block + header_unpack_size(block) < free_list->start + free_list->size / sizeof(word_t));
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
    printf("\n\n### START GC ###\n");
    fflush(stdout);
    clock_t start = clock(), diff;

    mark_roots();
    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);
    start = clock();
    sweep();
    diff = clock() - start;
    msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);
    printf("### END GC ###\n");
    fflush(stdout);
}

void memory_check(int print) {
    Bitmap* bitmap = free_list->bitmap;
    word_t* current = bitmap->offset;
    word_t* previous = NULL;
    size_t previous_size = 0;

    for(int i=0; i < bitmap->size / sizeof(word_t); i++) {
        if(bitmap_get_bit(bitmap, current)) {
            size_t size = header_unpack_size(current);
            if(header_unpack_tag(current) == tag_allocated && print) {
                printf("|A %p %zu", current, size);
            } else if(header_unpack_tag(current) == tag_free && print) {
                printf("|F %p %zu", current, size);
            }
            fflush(stdout);
            if(previous != NULL) {
                assert(previous + previous_size + 1 == current);
            } else {
                assert(current == bitmap->offset);
            }
            previous_size = size;
            previous = current;
        }
        current += 1;
    }
}
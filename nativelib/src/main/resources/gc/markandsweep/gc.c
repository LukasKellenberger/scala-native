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
#include "heap_sanity.h"

#define UNW_LOCAL_ONLY

#include <libunwind.h>
#include <setjmp.h>

FreeList* free_list = NULL;
Heap* heap_ = NULL;

#define CHUNK 512*1024*1024


void mark(word_t* block);


void memory_check(int print);

void print_rtti(word_t* block) {
    Rtti* rtti = *((Rtti**)(block + 1));
    if(rtti != NULL) {
        printf("rtti: %d\n", rtti->id);
    }
}

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
            //print_rtti(current);
            // Block is not alive, merge with next while possible
            word_t* next = heap_next_block(heap_, current);
            while(next != NULL && bitmap_get_bit(heap_->bitmap, next)) {
                //print_rtti(next);
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
    size = ((size + 8 - 1 ) / 8 * 8);
    size_t nb_words_with_header = (size / sizeof(word_t)) + 1;
    if(free_list == NULL) {
        scalanative_init();
    }
    //printf("alloc: %zu\n", nb_words_with_header);
    fflush(stdout);
    //free_list_print(free_list);
    //free_list_check(free_list);
    word_t* block = free_list_get_block(free_list, nb_words_with_header);
    //heap_sanity_full_check(heap_);

    //free_list_check(free_list);
    //free_list_print(free_list);
    fflush(stdout);
    assert(block == NULL || header_unpack_size(block) >= nb_words_with_header && header_unpack_size(block) <= nb_words_with_header + 1);
    assert(block == NULL || header_unpack_tag(block) == tag_allocated);
    if(block == NULL) {
        scalanative_collect();
        block = free_list_get_block(free_list, nb_words_with_header);
            assert(block == NULL || header_unpack_size(block) >= nb_words_with_header && header_unpack_size(block) <= nb_words_with_header + 1);
            assert(block == NULL || header_unpack_tag(block) == tag_allocated);
        if(block == NULL) {
            free_list_print_stats(free_list);

            printf("size: %zu\n", size);
            printf("No more memory available\n");
            fflush(stdout);
            exit(1);
        }
        memset(block + 1, 0, size - sizeof(word_t));
        return block + 1;
    }

    assert(block + header_unpack_size(block) <= free_list->start + free_list->size / sizeof(word_t));
    memset(block+1, 0, size - sizeof(word_t));

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

    //bitmap_print_with_rtti(free_list->bitmap);


    clock_t start = clock(), diff;
    mark_roots(heap_);
    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);

    //bitmap_print_with_rtti(free_list->bitmap);

    start = clock();
    sweep();
    diff = clock() - start;
    msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);
    memory_check(0);
    free_list_check(free_list);
    //free_list_print_stats(free_list);
    //free_list_print(free_list);
    //heap_sanity_full_check(heap_);

    //bitmap_print_with_rtti(free_list->bitmap);

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
                assert(previous + previous_size == current);
            } else {
                assert(current == bitmap->offset);
            }
            previous_size = size;
            previous = current;
        }
        current += 1;
    }
}
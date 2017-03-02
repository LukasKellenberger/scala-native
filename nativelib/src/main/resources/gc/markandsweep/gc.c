//
// Created by Lukas Kellenberger on 01.03.17.
//

#include "gc.h"
#include "free_list.h"
#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "linked_list.h"

#define UNW_LOCAL_ONLY

#include <libunwind.h>
#include <setjmp.h>

FreeList* free_list = NULL;

#define CHUNK 256*1024

#define DEBUG

void mark(word_t* block);
word_t* heap_start = NULL;
word_t* heap_end = NULL;

long count = 0;

inline static int in_heap(word_t* block) {
    return block >= heap_start && block < heap_end;
}

void _mark(word_t* block) {
    bitmap_clear_bit(free_list->bitmap, block);

    /*Rtti* rtti = get_rtti(block);
    printf("##### rtti size %ld\n", rtti->size);*/
    size_t size = header_unpack_size(block);

    for (int i = 0; i < size; i++) {
        word_t field = block[i + 1];
        word_t* field_addr = (word_t*)field-1;
        if(in_heap(field_addr)) {
            if (bitmap_get_bit(free_list->bitmap, field_addr)) {
                mark(field_addr);
            }
        }
    }
}
/*
 * Mark (pointer to header)
 */
void mark(word_t* block) {
    count++;
    // Check if pointer is on block header

    tag_t tag = header_unpack_tag(block);
    if (bitmap_get_bit(free_list->bitmap, block) && tag == tag_allocated) {
        _mark(block);
    } else {
        /*word_t* start = free_list->list->start
        //printf("could be inner pointer\n");
        word_t* current = block;
        while(current >= start && !bitmap_get_bit(bitmap_copy, addr_p_to_v(current))) {
            current -= 1;
        }

        if(current >= start) {

            tag_t tag = header_unpack_tag(*current);
            if(bitmap_get_bit(bitmap, addr_p_to_v(current)) && tag == tag_Allocated_Block) {
                _mark(current, addr_p_to_v(current));
            }
        }*/
    }
}

void mark_roots() {
    unw_cursor_t cursor;
    unw_context_t context;

    setjmp;
    // Initialize cursor to current frame for local unwinding.
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);
    // Unwind frames one by one, going up the frame stack.
    while (unw_step(&cursor) > 0) {
        unw_word_t rbp, rsp;

        unw_get_reg(&cursor, UNW_X86_64_RBP, &rbp);
        unw_get_reg(&cursor, UNW_X86_64_RSP, &rsp);

        unw_word_t p = rsp;
        while(p < rbp) {
            word_t* pp = *(word_t**)p;
            if(in_heap(pp)) {
                mark(pp - 1);
                p += 4;
            } else {
                p += 4;
            }
        }
    }

    /*word_t** module = __modules__;
    int nb_modules = __modules_size__;

    for(int i=0; i < nb_modules; i++) {
        word_t* addr = module[i] - 1;
        //printf("mark module %p\n", addr);
        fflush(stdout);
        if(in_heap(addr)) {
            mark(addr);
        }
    }*/
}

void sweep() {
    word_t* current = free_list->list->start;
    word_t* previous_list_elem = NULL;

    free_list->list->first = NULL;

    word_t* end = free_list->list->start + (free_list->list->size / sizeof(word_t));

    while(current < end) {

        size_t size = header_unpack_size(current) ;
        //Bit set means that it was not mark and thus not reachable, add it to free_list
        if(bitmap_get_bit(free_list->bitmap, current)) {
            #if defined(DEBUG)
                memset(current+1, 0, size * sizeof(word_t));
            #endif
            previous_list_elem = free_list_add_block(free_list, current, previous_list_elem);
        } else {
            //printf("not add block %zu\n", unpack_size(current));
            bitmap_set_bit(free_list->bitmap, current);
        }
        size_t current_size_h = size + 1;
        current += current_size_h;
    }

    //free_list_print(free_list);
}

void scalanative_init() {
    free_list = free_list_create(CHUNK);
    LinkedList* linkedList = free_list->list;
    heap_start = linkedList->start;
    heap_end = heap_start + (linkedList->size / sizeof(word_t));
}


void* scalanative_alloc_raw(size_t size) {
    size = ((size + sizeof(word_t) - 1 ) / sizeof(word_t)) * sizeof(word_t);
    if(free_list == NULL) {
        scalanative_init();
    }

    size_t size_with_header = size + sizeof(word_t);

    //printf("*** ALLOC size: %zu ***\n", size_with_header);

    word_t* block = free_list_get_block(free_list, size_with_header);
    if(block == NULL) {
        scalanative_collect();
        block = free_list_get_block(free_list, size_with_header);
        if(block == NULL) {
            printf("No more memory available\n");
            fflush(stdout);
            exit(1);
        }
        return block + 1;
    }
    //free_list_print(free_list);
    //bitmap_print(bitmap, 1000);
    //printf("*** END ALLOC ***\n");

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
    count = 0;
    printf("\n\n### START GC ###\n");
    free_list_print_stats(free_list);
    free_list_print(free_list);
    //print_memory(free_list);
    fflush(stdout);
    /*size_t size = CHUNK * sizeof(word_t);
    unsigned long nb_words = (size + BITS_PER_WORD - 1) / BITS_PER_WORD;
    memcpy(bitmap_copy, bitmap, nb_words * sizeof(word_t));*/

    mark_roots();
    //free_list_print(free_list);
    //bitmap_print(bitmap, 1000);
    //print_mem();
    sweep();
    free_list_print(free_list);
    free_list_print_stats(free_list);
    printf("count: %ld\n", count);
    printf("### END GC ###\n");
    fflush(stdout);

    //free_list_print(free_list);
    //print_mem();
}
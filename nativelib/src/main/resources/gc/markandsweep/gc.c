//
// Created by Lukas Kellenberger on 01.03.17.
//

#include "gc.h"
#include "free_list.h"
#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "linked_list.h"
#include <assert.h>

#define UNW_LOCAL_ONLY

#include <libunwind.h>
#include <setjmp.h>

FreeList* free_list = NULL;

#define CHUNK 256*1024*1024

#define DEBUG

void mark(word_t* block);
word_t* heap_start = NULL;
word_t* heap_end = NULL;

Bitmap* bitmap_copy = NULL;

long count = 0;
long count2 = 0;
long count3 = 0;

extern word_t** __MODULES__;
extern int __MODULES_SIZE__;

inline static int in_heap(word_t* block) {
    return block >= heap_start && block < heap_end;
}

void memory_check();
void mark(word_t* block);

void mark_inner(word_t* inner_ptr) {
    word_t* current = inner_ptr - 1;
    while(!bitmap_get_bit(bitmap_copy, current)) {
        current -= 1;
    }
    size_t size = header_unpack_size(current);
    if(current + size <= inner_ptr) {
        mark(current);
    }
}

void _mark(word_t* block) {
    assert(bitmap_get_bit(free_list->bitmap, block));
    assert(header_unpack_tag(block) == tag_allocated);
    assert(in_heap(block));

    bitmap_clear_bit(free_list->bitmap, block);


    Rtti rtti = *((Rtti*) *(block+1));
    //printf("%p\n", block);
    //printf("#### id: %d\n", rtti.id);
    /*printf("##### rtti size %lld\n", rtti.size);
    printf("### offset: %p\n", rtti.ptr_map);
    fflush(stdout);*/

    size_t size = header_unpack_size(block);
    assert(size < heap_end - heap_start);

    /*int64_t* ptr_map = rtti.ptr_map;
    int i=0;
    while(ptr_map[i] != -1) {
        assert(ptr_map[i] % 8 == 0);
        word_t field = block[ptr_map[i]/sizeof(word_t) + 1];
        word_t* field_addr = (word_t*)field - 1;
        if(in_heap(field_addr)) {
            if (bitmap_get_bit(free_list->bitmap, field_addr)) {
                mark(field_addr);
            } else {
                count2++;
            }
        }
        ++i;
    }*/



    for (int i = 0; i < size - 1; i++) {
        word_t field = block[i + 2];
        word_t* field_addr = (word_t*)field-1;
        if(in_heap(field_addr)) {
            if (bitmap_get_bit(free_list->bitmap, field_addr)) {
                mark(field_addr);
            } else if(!bitmap_get_bit(bitmap_copy, block)) {
                count2++;
                mark_inner(field_addr);
            } else {
                count3++;
            }
        }
    }
}
/*
 * Mark (pointer to header)
 */
void mark(word_t* block) {
    // Check if pointer is on block header

    tag_t tag = header_unpack_tag(block);
    if (bitmap_get_bit(free_list->bitmap, block) && tag == tag_allocated) {
        count++;
        _mark(block);
    } else if(!bitmap_get_bit(bitmap_copy, block)) {
        printf("inner ptr\n");
        count2++;
        mark_inner(block);
    } else {
        count3++;
    }
}

void mark_roots_registers(unw_cursor_t* cursor) {
    unw_word_t reg;

    int regs[] = {UNW_X86_64_RAX,
                  UNW_X86_64_RDX,
                  UNW_X86_64_RCX,
                  UNW_X86_64_RBX,
                  UNW_X86_64_RSI,
                  UNW_X86_64_RDI,
                  UNW_X86_64_RBP,
                  UNW_X86_64_RSP,
                  UNW_X86_64_R8,
                  UNW_X86_64_R9,
                  UNW_X86_64_R10,
                  UNW_X86_64_R11,
                  UNW_X86_64_R12,
                  UNW_X86_64_R13,
                  UNW_X86_64_R14,
                  UNW_X86_64_R15};

    for(int i=0; i < 17; i++) {
        unw_get_reg(cursor, regs[i], &reg);
        word_t* pp = (word_t*)reg - 1;
        if(in_heap(pp)) {
            mark(pp);
        }
    }
}

void mark_roots_stack(unw_cursor_t* cursor) {
    unw_word_t top = 0, bottom = 0;//0x7fff5fc00000;
    while (unw_step(cursor) > 0) {
        unw_word_t rbp, rsp, offset;

        unw_get_reg(cursor, UNW_X86_64_RBP, &rbp);
        unw_get_reg(cursor, UNW_X86_64_RSP, &rsp);
        /*printf("rsp: %p\n", rsp);
        printf("rbp: %p\n", rbp);
        printf("top: %p\n", top);
        printf("bottom: %p\n", bottom);*/

        char sym[256];
        //unw_get_proc_name(cursor, sym, sizeof(sym), &offset);
        //printf(" (%s+0x%lx)\n", sym, offset);
        //fflush(stdout);

        if(top == 0 || rsp < top) {
            top = rsp;
        }
        if(bottom == 0 || rbp > bottom) {
            bottom = rbp;
        }

        if(rsp > bottom) {
            bottom = rsp;
        }
    }
    unw_word_t p = top;

    while(p < bottom) {

        word_t* pp = (*(word_t**)p) - 1;
        if(in_heap(pp)) {
            mark(pp);
        }
        p += 8;
    }
}

void mark_roots_modules() {
    word_t** module = __MODULES__;
    int nb_modules = __MODULES_SIZE__;

    for(int i=0; i < nb_modules; i++) {
        word_t* addr = module[i] - 1;
        if(in_heap(addr)) {
            mark(addr);
        }
    }
}

void mark_roots() {
    unw_cursor_t cursor;
    unw_context_t context;

    //setjmp;
    // Initialize cursor to current frame for local unwinding.
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    mark_roots_registers(&cursor);
    mark_roots_stack(&cursor);
    mark_roots_modules();
}

void sweep() {
    word_t* current = free_list->list->start;
    word_t* previous_list_elem = NULL;

    assert(current + header_unpack_size(current) <= free_list->list->start + free_list->list->size/sizeof(word_t));


    free_list->list->first = NULL;

    word_t* end = free_list->list->start + (free_list->list->size / sizeof(word_t));

    size_t total = 0;
    size_t heap_size = free_list->list->size / sizeof(word_t);

    while(current < end) {
        size_t previous_size = previous_list_elem == NULL ? 0 : header_unpack_size(previous_list_elem);
        size_t size = header_unpack_size(current);
        //Bit set means that it was not mark and thus not reachable, add it to free_list
        if(bitmap_get_bit(free_list->bitmap, current)) {
            #if defined(DEBUG)
                memset(current+1, 0, size * sizeof(word_t));
            #endif
            previous_list_elem = free_list_add_block(free_list, current, previous_list_elem);
             if(previous_list_elem != NULL) {
                total += header_unpack_size(previous_list_elem) + 1;
                assert(size == header_unpack_size(previous_list_elem) || size + previous_size + 1 == header_unpack_size(previous_list_elem));
             }
        } else {
            bitmap_set_bit(free_list->bitmap, current);
        }
        size_t current_size_h = size + 1;
        current += current_size_h;

        //assert(total <= heap_size);

    }

    //free_list_print(free_list);
}

void scalanative_init() {
    free_list = free_list_create(CHUNK);
    LinkedList* linkedList = free_list->list;
    heap_start = linkedList->start;
    heap_end = heap_start + (linkedList->size / sizeof(word_t));

    Bitmap* bitmap = free_list->bitmap;
    bitmap_copy = bitmap_alloc(bitmap->size, bitmap->offset);
}


void* scalanative_alloc_raw(size_t size) {
    size = (size + 16 - 1 ) / 16 * 16;
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
            free_list_print(free_list);
            printf("size: %zu\n", size);
            printf("No more memory available\n");
            fflush(stdout);
            exit(1);
        }
        memset(block+1, 0, size);
        return block + 1;
    }
    //free_list_print(free_list);
    //bitmap_print(bitmap, 1000);
    //printf("*** END ALLOC ***\n");

    assert(block + header_unpack_size(block) < heap_end);
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
    memory_check();
    bitmap_clone(free_list->bitmap, bitmap_copy);
    count = 0;
    count2 = 0;
    count3 = 0;
    printf("\n\n### START GC ###\n");


    //free_list_print_stats(free_list);
    //free_list_print(free_list);
    //print_memory(free_list);
    fflush(stdout);

    mark_roots();
    //free_list_print(free_list);
    //bitmap_print(bitmap, 1000);
    //print_mem();
    sweep();
    //free_list_print(free_list);
    //free_list_print_stats(free_list);
    printf("count: %ld\n", count);
    printf("inner ptrs: %ld\n", count2);
    printf("else ptrs: %ld\n", count3);

    printf("### END GC ###\n");
    fflush(stdout);
    //memory_check();


    //free_list_print(free_list);
    //print_mem();
}

void memory_check() {
    Bitmap* bitmap = free_list->bitmap;
    word_t* current = bitmap->offset;
    word_t* previous = NULL;
    size_t previous_size = 0;

    for(int i=0; i < bitmap->size / sizeof(word_t); i++) {
        if(bitmap_get_bit(bitmap, current)) {
            if(previous != NULL) {
                assert(previous + previous_size + 1 == current);
            } else {
                assert(current == bitmap->offset);
            }
            previous_size = header_unpack_size(current);
            previous = current;
        }
        current += 1;
    }
}
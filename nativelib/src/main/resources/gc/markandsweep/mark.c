
#include "mark.h"


void mark(word_t* block);

Bitmap* bitmap = NULL;
Bitmap* bitmap_copy = NULL;

word_t* heap_start = NULL;
word_t* heap_end = NULL;



inline static int in_heap(word_t* block) {
    return block >= heap_start && block < heap_end;
}

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
    assert(bitmap_get_bit(bitmap, block));
    assert(header_unpack_tag(block) == tag_allocated);
    assert(in_heap(block));

    bitmap_clear_bit(bitmap, block);


    Rtti rtti = *((Rtti*) *(block+1));

    if(rtti.id == __OBJECT_ARRAY_ID__) {
        size_t size = header_unpack_size(block);
        assert(size < heap_end - heap_start);

        for (int i = 0; i < size - 1; i++) {
            word_t field = block[i + 2];
            word_t* field_addr = (word_t*)field-1;
            if(in_heap(field_addr)) {
                if (bitmap_get_bit(bitmap, field_addr)) {
                    mark(field_addr);
                } else if(!bitmap_get_bit(bitmap_copy, block)) {
                    mark_inner(field_addr);
                }
            }
        }
    } else {
        int64_t* ptr_map = rtti.ptr_map;
        int i=0;
        while(ptr_map[i] != -1) {
            assert(ptr_map[i] % 8 == 0);
            word_t field = block[ptr_map[i]/sizeof(word_t) + 1];
            word_t* field_addr = (word_t*)field - 1;
            if(in_heap(field_addr)) {
                if (bitmap_get_bit(bitmap, field_addr)) {
                    mark(field_addr);
                } else if(!bitmap_get_bit(bitmap_copy, block))  {
                    mark_inner(field_addr);
                }
            }
            ++i;
        }
    }
}
/*
 * Mark (pointer to header)
 */
void mark(word_t* block) {
    // Check if pointer is on block header

    tag_t tag = header_unpack_tag(block);
    if (bitmap_get_bit(bitmap, block) && tag == tag_allocated) {
        _mark(block);
    } else if(!bitmap_get_bit(bitmap_copy, block)) {
        mark_inner(block);
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
    unw_word_t top = LONG_MAX, bottom = 0;
    unw_word_t rsp;
    while (unw_step(cursor) > 0) {

        unw_get_reg(cursor, UNW_X86_64_RSP, &rsp);

        if(rsp < top) {
            top = rsp;
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

    bitmap_clone(bitmap, bitmap_copy);


    //setjmp;
    // Initialize cursor to current frame for local unwinding.
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    mark_roots_registers(&cursor);
    mark_roots_stack(&cursor);
    mark_roots_modules();
}

void mark_init(FreeList* free_list) {
    bitmap = free_list->bitmap;
    bitmap_copy = bitmap_alloc(bitmap->size, bitmap->offset);
    heap_start = free_list->start;
    heap_end = heap_start + free_list->size / sizeof(word_t);
}
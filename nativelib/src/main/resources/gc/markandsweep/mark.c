
#include "mark.h"


void mark(word_t* block);

Bitmap* bitmap = NULL;
Bitmap* bitmap_copy = NULL;

word_t* heap_start = NULL;
word_t* heap_end = NULL;
Stack* stack = NULL;
word_t* overflow_current_addr = NULL;
int overflow = 0;

inline static int cannot_be_const(word_t* block) {
    return block > heap_start;
}

inline static int in_heap(word_t* block) {
    return block >= heap_start && block < heap_end;
}

word_t* inner_get_header(word_t* inner_ptr) {
    word_t* current = inner_ptr - 1;
    while(!bitmap_get_bit(bitmap_copy, current)) {
        current -= 1;
    }
    size_t size = header_unpack_size(current);
    if(current + size <= inner_ptr && header_unpack_tag(current) == tag_allocated) {
        return current;
    }

    return NULL;
}

void scan_heap_after_overflow(Stack* stack) {

    word_t* block = overflow_current_addr;
    int found = 0;
    while(block < heap_end && !found) {
        assert(bitmap_get_bit(bitmap_copy, block));
        if(!bitmap_get_bit(bitmap, block) && header_unpack_tag(block) == tag_allocated) {
            Rtti rtti = *((Rtti*) *(block+1));
            if(rtti.id == __OBJECT_ARRAY_ID__) {

                size_t size = header_unpack_size(block);

                for (int i = 0; i < size - 1; i++) {
                    word_t* field = (word_t*)(block[i + 2]);
                    if(field != NULL && cannot_be_const(field)) {
                        word_t* field_addr = field - 1;
                        if (bitmap_get_bit(bitmap, field_addr)) {
                            stack_push(stack, block);
                            found = 1;
                            break;
                        } else if(!bitmap_get_bit(bitmap_copy, field_addr)) {
                            word_t* inner_header = inner_get_header(field_addr);
                            if(inner_header != NULL) {
                                stack_push(stack, block);
                                found = 1;
                                break;
                            }
                        }
                    }
                }
            } else {
                int64_t* ptr_map = rtti.ptr_map;
                int i=0;
                while(ptr_map[i] != -1) {
                    assert(ptr_map[i] % 8 == 0);
                    word_t* field = (word_t*)(block[ptr_map[i]/sizeof(word_t) + 1]);
                    if(field != NULL && cannot_be_const(field)) {
                        word_t* field_addr = (word_t*)field - 1;
                        if (bitmap_get_bit(bitmap, field_addr)) {
                            stack_push(stack, block);
                            found = 1;
                            break;
                        } else if(!bitmap_get_bit(bitmap_copy, field_addr))  {
                            word_t* inner_header = inner_get_header(field_addr);
                            if(inner_header != NULL) {
                                stack_push(stack, block);
                                found = 1;
                                break;
                            }
                        }
                    }
                    ++i;
                }
            }
        }
        block += header_unpack_size(block) + 1;
    }
    overflow_current_addr = block;

}

void _mark() {
    while(!stack_is_empty(stack)) {
        word_t* block = stack_pop(stack);
        assert(header_unpack_tag(block) == tag_allocated);
        assert(in_heap(block));

        Rtti rtti = *((Rtti*) *(block+1));

        if(rtti.id == __OBJECT_ARRAY_ID__) {
            size_t size = header_unpack_size(block);
            assert(size < heap_end - heap_start);

            for (int i = 0; i < size - 1; i++) {
                word_t* field = (word_t*)(block[i + 2]);
                if(field != NULL && cannot_be_const(field)) {
                    word_t* field_addr = field - 1;
                    if (bitmap_get_bit(bitmap, field_addr)) {
                        bitmap_clear_bit(bitmap, field_addr);
                        if(!overflow) {
                            overflow = stack_push(stack, field_addr);
                        }
                    } else if(!bitmap_get_bit(bitmap_copy, field_addr)) {
                        word_t* inner_header = inner_get_header(field_addr);
                        if(inner_header != NULL) {
                            bitmap_clear_bit(bitmap, inner_header);
                            if(!overflow) {
                                overflow = stack_push(stack, inner_header);
                            }
                        }
                    }
                }
            }
        } else {
            int64_t* ptr_map = rtti.ptr_map;
            int i=0;
            while(ptr_map[i] != -1) {
                assert(ptr_map[i] % 8 == 0);
                word_t* field = (word_t*)(block[ptr_map[i]/sizeof(word_t) + 1]);
                if(field != NULL && cannot_be_const(field)) {
                    word_t* field_addr = (word_t*)field - 1;
                    if (bitmap_get_bit(bitmap, field_addr)) {
                        bitmap_clear_bit(bitmap, field_addr);
                        if(!overflow) {
                            overflow = stack_push(stack, field_addr);
                        }
                    } else if(!bitmap_get_bit(bitmap_copy, field_addr))  {
                        word_t* inner_header = inner_get_header(field_addr);
                        if(inner_header != NULL) {
                            bitmap_clear_bit(bitmap, inner_header);
                            if(!overflow) {
                                overflow = stack_push(stack, inner_header);
                            }
                        }
                    }
                }
                ++i;
            }
        }
    }
    if(overflow) {
        overflow = 0;
        stack_double_size(stack);
        printf("double size\n");
        while(overflow_current_addr < heap_end) {
            scan_heap_after_overflow(stack);
            _mark();
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
        if(!overflow) {
            overflow = stack_push(stack, block);
        }
        bitmap_clear_bit(bitmap, block);
    } else if(!bitmap_get_bit(bitmap_copy, block)) {
        block = inner_get_header(block);
        if(block != NULL) {
            if(!overflow) {
                overflow = stack_push(stack, block);
            }
            bitmap_clear_bit(bitmap, block);
        }
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

    overflow_current_addr = heap_start;
    mark_roots_registers(&cursor);
    mark_roots_stack(&cursor);
    mark_roots_modules();
    _mark();
}

void mark_init(FreeList* free_list) {
    bitmap = free_list->bitmap;
    bitmap_copy = bitmap_alloc(bitmap->size, bitmap->offset);
    stack = stack_alloc(2*1024*1024);
    heap_start = free_list->start;
    heap_end = heap_start + free_list->size / sizeof(word_t);
}
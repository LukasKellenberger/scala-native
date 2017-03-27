
#include "mark.h"

#define INITIAL_STACK_SIZE 512*1024

void mark(word_t* block);

Heap* heap = NULL;
Stack* stack = NULL;
word_t* overflow_current_addr = NULL;
int overflow = 0;

word_t* inner_get_header(word_t* inner_ptr) {
    word_t* current = inner_ptr - 1;
    while(!bitmap_get_bit(heap->bitmap_copy, current)) {
        current -= 1;
    }
    size_t size = header_unpack_size(current);
    if(current + size >= inner_ptr && header_unpack_tag(current) == tag_allocated && bitmap_get_bit(heap->bitmap, current)) {
        return current;
    }

    return NULL;
}

void scan_heap_after_overflow(Stack* stack) {

    word_t* block = overflow_current_addr;
    int found = 0;
    while(block != NULL && !found) {
        assert(bitmap_get_bit(heap->bitmap_copy, block));
        if(!bitmap_get_bit(heap->bitmap, block) && header_unpack_tag(block) == tag_allocated) {
            Rtti rtti = *((Rtti*) *(block+1));
            if(rtti.id == __OBJECT_ARRAY_ID__) {

                size_t size = header_unpack_size(block);

                for (int i = 0; i < size - 1; i++) {
                    word_t* field = (word_t*)(block[i + 2]);
                    word_t* field_addr = field - 1;
                    if(heap_in_heap(heap, field_addr)) {
                        if (bitmap_get_bit(heap->bitmap, field_addr)) {
                            assert(heap_in_heap(heap, field_addr));
                            stack_push(stack, block);
                            found = 1;
                            break;
                        } else if(!bitmap_get_bit(heap->bitmap_copy, field_addr)) {
                            word_t* inner_header = inner_get_header(field_addr);
                            if(inner_header != NULL) {
                                assert(heap_in_heap(heap, field_addr));
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
                    word_t* field_addr = (word_t*)field - 1;
                    if(heap_in_heap(heap, field_addr)) {
                        if (bitmap_get_bit(heap->bitmap, field_addr)) {
                            assert(heap_in_heap(heap, field_addr));
                            stack_push(stack, block);
                            found = 1;
                            break;
                        } else if(!bitmap_get_bit(heap->bitmap_copy, field_addr))  {
                            word_t* inner_header = inner_get_header(field_addr);
                            if(inner_header != NULL) {
                                assert(heap_in_heap(heap, inner_header));
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
        block = heap_next_block(heap, block);
    }
    overflow_current_addr = block;

}

void _mark() {
    while(!stack_is_empty(stack)) {
        word_t* block = stack_pop(stack);
        assert(header_unpack_tag(block) == tag_allocated);
        assert(heap_in_heap(heap, block));

        Rtti rtti = *((Rtti*) *(block+1));

        if(rtti.id == __OBJECT_ARRAY_ID__) {
            size_t size = header_unpack_size(block);
            assert(size < heap->heap_end - heap->heap_start);

            for (int i = 0; i < size - 1; i++) {
                word_t* field = (word_t*)(block[i + 2]);
                word_t* field_addr = field - 1;
                if(heap_in_heap(heap, field_addr)) {
                    if (bitmap_get_bit(heap->bitmap, field_addr) && header_unpack_tag(field_addr) == tag_allocated) {
                        bitmap_clear_bit(heap->bitmap, field_addr);
                        if(!overflow) {
                            assert(header_unpack_tag(field_addr) == tag_allocated);
                            overflow = stack_push(stack, field_addr);
                        }
                    } else if(!bitmap_get_bit(heap->bitmap_copy, field_addr)) {
                        word_t* inner_header = inner_get_header(field_addr);
                        if(inner_header != NULL) {
                            bitmap_clear_bit(heap->bitmap, inner_header);
                            if(!overflow) {
                                assert(heap_in_heap(heap, inner_header));
                                assert(header_unpack_tag(inner_header) == tag_allocated);
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
                word_t* field_addr = (word_t*)field - 1;
                if(heap_in_heap(heap, field_addr)) {
                    if (bitmap_get_bit(heap->bitmap, field_addr)) {
                        bitmap_clear_bit(heap->bitmap, field_addr);
                        if(!overflow) {
                            assert(heap_in_heap(heap, field_addr));
                            assert(header_unpack_tag(field_addr) == tag_allocated);
                            overflow = stack_push(stack, field_addr);
                        }
                    } else if(!bitmap_get_bit(heap->bitmap_copy, field_addr))  {
                        word_t* inner_header = inner_get_header(field_addr);
                        if(inner_header != NULL) {
                            bitmap_clear_bit(heap->bitmap, inner_header);
                            if(!overflow) {
                                assert(header_unpack_tag(inner_header) == tag_allocated);
                                assert(heap_in_heap(heap, inner_header));
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
        overflow_current_addr = heap->heap_start;
        overflow = 0;
        stack_double_size(stack);
        printf("double size\n");
        while(overflow_current_addr != NULL) {
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
    if (bitmap_get_bit(heap->bitmap, block) && tag == tag_allocated) {
        if(!overflow) {
            assert(heap_in_heap(heap, block));
            overflow = stack_push(stack, block);
        }
        bitmap_clear_bit(heap->bitmap, block);
    } else if(!bitmap_get_bit(heap->bitmap_copy, block)) {
        block = inner_get_header(block);
        if(block != NULL) {
            if(!overflow) {
                assert(heap_in_heap(heap, block));
                assert(header_unpack_tag(block) == tag_allocated);
                overflow = stack_push(stack, block);
            }
            bitmap_clear_bit(heap->bitmap, block);
        }
    }
}

void mark_roots_stack() {
    unw_cursor_t cursor;
    unw_context_t context;
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);
    unw_word_t top = LONG_MAX, bottom = 0;
    unw_word_t rsp;

    while (unw_step(&cursor) > 0) {
        unw_get_reg(&cursor, UNW_X86_64_RSP, &rsp);

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
        if(heap_in_heap(heap, pp)) {
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
        if(heap_in_heap(heap, addr)) {
            mark(addr);
        }
    }
}

void print_stack() {
    for(int i=0; i < stack->current; i++) {
        word_t* block = stack->bottom[i];
        Rtti* rtti = *(Rtti**)(block + 1);
        printf("%d %lu %p\n",rtti->id, block - heap->heap_start, block);
    }
}

void mark_roots(Heap* _heap) {
    if(stack == NULL) {
        stack = stack_alloc(INITIAL_STACK_SIZE);
    }

    heap = _heap;

    bitmap_clone(heap->bitmap, heap->bitmap_copy);
    // Dumps registers into 'regs' which is on stack
    jmp_buf regs;
    setjmp(regs);


    overflow_current_addr = heap->heap_start;
    mark_roots_stack();
    //print_stack();
    mark_roots_modules();
    _mark();
}

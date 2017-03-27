
#include "mark.h"

#define INITIAL_STACK_SIZE 16*1024*1024

void mark(word_t* block);

Heap* heap = NULL;
Stack* stack = NULL;
word_t* overflow_current_addr = NULL;
int overflow = 0;
int inner = 0;

/*
 * Used to find the header of an inner pointer. Returns the block header if
 * the inner pointer was inside a live block that is not marked yet.
 */
word_t* inner_get_header(word_t* inner_ptr) {
    inner++;
    // Starting at word above the inner pointer
    uintptr_t ptr = (uintptr_t)inner_ptr;
    uintptr_t aligned_ptr = ptr - (ptr % sizeof(word_t));
    word_t* current = (word_t*) aligned_ptr;

    // Go up while the block is not a header. It uses the bitmap_copy, because bitmap clears bits for marked blocks.
    // No need to check for heap bounds because heap_start is a header.
    while(!bitmap_get_bit(heap->bitmap_copy, current)) {
        current -= 1;
    }

    // Need to check if the block is allocated and if it is not marked
    if(header_unpack_tag(current) == tag_allocated && bitmap_get_bit(heap->bitmap, current)) {
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

                size_t size = header_unpack_size(block) - 1;

                for (int i = 0; i < size - 1; i++) {
                    word_t* field = (word_t*)(block[i + 2]);
                    word_t* field_addr = field - 1;
                    if(heap_in_heap(heap, field_addr) && bitmap_get_bit(heap->bitmap, field_addr) && header_unpack_tag(field_addr) == tag_allocated) {
                        assert(heap_in_heap(heap, field_addr));
                        stack_push(stack, block);
                        found = 1;
                        break;
                    }
                }
            } else {
                int64_t* ptr_map = rtti.ptr_map;
                int i=0;
                while(ptr_map[i] != -1) {
                    assert(ptr_map[i] % 8 == 0);
                    word_t* field = (word_t*)(block[ptr_map[i]/sizeof(word_t) + 1]);
                    word_t* field_addr = (word_t*)field - 1;
                    if(heap_in_heap(heap, field_addr) && bitmap_get_bit(heap->bitmap, field_addr) && header_unpack_tag(field_addr) == tag_allocated) {
                        assert(heap_in_heap(heap, field_addr));
                        stack_push(stack, block);
                        found = 1;
                        break;
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
        assert(bitmap_get_bit(heap->bitmap_copy, block));
        assert(header_unpack_tag(block) == tag_allocated);
        assert(heap_in_heap(heap, block));
        assert(header_unpack_size(block) > 1);

        Rtti rtti = *((Rtti*) *(block+1));

        if(rtti.id == __OBJECT_ARRAY_ID__) {
            size_t size = header_unpack_size(block) - 1;
            assert(size < heap->heap_end - heap->heap_start);

            for (int i = 0; i < size - 1; i++) {
                word_t* field = (word_t*)(block[i + 2]);
                word_t* field_addr = field - 1;
                if(heap_in_heap(heap, field_addr) && bitmap_get_bit(heap->bitmap, field_addr) && header_unpack_tag(field_addr) == tag_allocated) {
                    bitmap_clear_bit(heap->bitmap, field_addr);
                    assert(header_unpack_tag(field_addr) == tag_allocated);
                    assert(header_unpack_size(field_addr) > 1);
                    if(!overflow) {
                        overflow = stack_push(stack, field_addr);
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
                if(heap_in_heap(heap, field_addr) && bitmap_get_bit(heap->bitmap, field_addr) && header_unpack_tag(field_addr) == tag_allocated) {
                    bitmap_clear_bit(heap->bitmap, field_addr);
                    assert(heap_in_heap(heap, field_addr));
                    assert(header_unpack_tag(field_addr) == tag_allocated);
                    assert(header_unpack_size(field_addr) > 1);
                    if(!overflow) {
                        overflow = stack_push(stack, field_addr);
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

        #ifdef STACK_OVERFLOW_PRINT
            printf("Stack grew to %zu bytes\n", stack->nb_words * sizeof(Stack_Type));
        #endif

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
    if (((uintptr_t)block & (uintptr_t)0x7) == 0 && bitmap_get_bit(heap->bitmap, block) && tag == tag_allocated) {
        bitmap_clear_bit(heap->bitmap, block);
        assert(heap_in_heap(heap, block));
        assert(header_unpack_size(block) > 1);
        if(!overflow) {
            overflow = stack_push(stack, block);
        }
    } else if(!bitmap_get_bit(heap->bitmap_copy, block)) {
        block = inner_get_header(block);
        if(block != NULL) {
            bitmap_clear_bit(heap->bitmap, block);
            assert(heap_in_heap(heap, block));
            assert(header_unpack_tag(block) == tag_allocated);
            assert(header_unpack_size(block) > 1);
            if(!overflow) {
                overflow = stack_push(stack, block);
            }
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
    inner = 0;
    mark_roots_stack();
    mark_roots_modules();
    _mark();
    printf("inner: %d\n", inner);
}

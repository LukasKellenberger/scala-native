//
// Created by Lukas Kellenberger on 21.04.17.
//

#include <stdio.h>
#include "Marker.h"
#include "Object.h"

extern int __OBJECT_ARRAY_ID__;


void markObject(Stack* stack, ObjectHeader* object) {
    assert(!object_isMarked(object));
    //printf("\tadd: %p %d\n", object, object->rtti->id);
    //fflush(stdout);
    //size_t testSize = object_size((ObjectHeader*)0x0000000101008188);
    object_mark(object);
    stack_push(stack, object);
}

void mark(Heap* heap, Stack* stack, word_t* address) {
    assert(heap_isWordInHeap(heap, address));
    ObjectHeader* object = NULL;
    if(heap_isWordInSmallHeap(heap, address)) {
        object = object_getObject(address);
    } else {
        object = object_getLargeObject(heap->largeAllocator, address);
    }

    if(object != NULL && !object_isMarked(object)) {
        markObject(stack, object);
    } /*else if(object == NULL) {
        printf("Not marking: %p\n", address);
    }*/
}

void markModules(Heap* heap, Stack* stack, word_t** modules, int moduleCount) {
    for(int i=0; i < moduleCount; i++) {
        word_t* module = modules[i] - 1;
        if(heap_isWordInHeap(heap, module)) {
            mark(heap, stack, module);
        }
    }
}

void markStack(Heap* heap, Stack* stack, word_t* stackTop, word_t* stackBottom) {

    while(stackTop < stackBottom) {

        word_t* object = (*(word_t**)stackTop) - 1;
        if(heap_isWordInHeap(heap, object)) {
            mark(heap, stack, object);
        }
        stackTop++;
    }
}

void marker_markRoots(Heap* heap, Roots* roots, Stack* stack) {
    assert(stack_isEmpty(stack));
    markStack(heap, stack, roots->stackTop, roots->stackBottom);
    markModules(heap, stack, roots->moduleStart, roots->moduleCount);
}

void marker_mark(Heap* heap, Stack* stack) {
    //printf("mark!\n");
    while(!stack_isEmpty(stack)) {
        ObjectHeader* object = stack_pop(stack);
        //printf("marking: %p %d\n", object, object->rtti->id);

        if(object->rtti->id == __OBJECT_ARRAY_ID__) {
            // remove header and rtti from size
            size_t size = object_size(object) - 2 * sizeof(word_t);
            size_t nbWords = size / sizeof(word_t);
            for(int i = 0; i < nbWords; i++) {

                word_t* field = object->fields[i];
                ObjectHeader* fieldObject = (ObjectHeader*)(field - 1);
                if(heap_isObjectInHeap(heap, fieldObject) && !object_isMarked(fieldObject)) {
                    markObject(stack, fieldObject);
                }

            }
        } else {
            int64_t* ptr_map = object->rtti->ptr_map;
            int i=0;
            while(ptr_map[i] != -1) {
                word_t* field = object->fields[ptr_map[i]/sizeof(word_t) - 1];
                ObjectHeader* fieldObject = (ObjectHeader*)(field - 1);
                if(heap_isObjectInHeap(heap, fieldObject) && !object_isMarked(fieldObject)) {
                    markObject(stack, fieldObject);
                }
                ++i;
            }
        }
    }
}

void mark_roots_stack(Heap* heap, Stack* stack) {
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
        if(heap_isWordInHeap(heap, pp)) {
            mark(heap, stack, pp);
        }
        p += 8;
    }
}

void mark_roots_modules(Heap* heap, Stack* stack) {
    word_t** module = __MODULES__;
    int nb_modules = __MODULES_SIZE__;

    for(int i=0; i < nb_modules; i++) {
        word_t* addr = module[i] - 1;
        if(heap_isWordInHeap(heap, addr)) {
            mark(heap, stack, addr);
        }
    }
}

void mark_roots(Heap* heap, Stack* stack) {

    // Dumps registers into 'regs' which is on stack
    jmp_buf regs;
    //memset(&regs, 0, sizeof(jmp_buf));
    setjmp(regs);

    mark_roots_stack(heap, stack);

    mark_roots_modules(heap, stack);

    marker_mark(heap, stack);

}
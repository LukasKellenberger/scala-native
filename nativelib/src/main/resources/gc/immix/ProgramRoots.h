//
// Created by Lukas Kellenberger on 20.04.17.
//

#ifndef IMMIX_PROGRAMROOTS_H
#define IMMIX_PROGRAMROOTS_H

#define UNW_LOCAL_ONLY


#include <setjmp.h>
#include <libunwind.h>
#include <limits.h>
#include <stdlib.h>
#include "headers/gc_types.h"



extern word_t** __MODULES__;
extern int __MODULES_SIZE__;

typedef struct {
    word_t* stackTop;
    word_t* stackBottom;
    word_t** moduleStart;
    int moduleCount;
} Roots;


static void ProgramRoots_getStackBottom(Roots* roots) {
    unw_cursor_t cursor;
    unw_context_t context;
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);
    unw_word_t bottom = 0;
    unw_word_t rsp;

    while (unw_step(&cursor) > 0) {
        unw_get_reg(&cursor, UNW_X86_64_RSP, &rsp);

        if(rsp > bottom) {
            bottom = rsp;
        }
    }

    roots->stackBottom = (word_t*)bottom;
}

static void ProgramRoots_getRoots(Roots** roots) {
    if(*roots == NULL) {
        *roots = malloc(sizeof(Roots));
        ProgramRoots_getStackBottom(*roots);
    }

    jmp_buf regs = {0};
    setjmp(regs);

    (*roots)->stackTop = (word_t*) &regs;
    (*roots)->moduleStart = __MODULES__;
    (*roots)->moduleCount = __MODULES_SIZE__;

}


#endif //IMMIX_PROGRAMROOTS_H

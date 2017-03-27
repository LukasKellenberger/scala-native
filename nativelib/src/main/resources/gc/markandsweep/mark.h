//
// Created by Lukas Kellenberger on 01.03.17.
//

#ifndef MARK_H
#define MARK_H

#include <stdio.h>
#include <limits.h>
#include <setjmp.h>
#include <libunwind.h>

#include "block.h"
#include "types.h"
#include "bitmap.h"
#include "free_list.h"
#include "stack.h"
#include "heap.h"

#define UNW_LOCAL_ONLY


extern word_t** __MODULES__;
extern int __MODULES_SIZE__;
extern int __OBJECT_ARRAY_ID__;

void mark_roots();
void mark_init(FreeList* free_list);

#endif // MARK_H
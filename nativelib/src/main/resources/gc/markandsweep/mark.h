//
// Created by Lukas Kellenberger on 01.03.17.
//

#ifndef MARK_H
#define MARK_H

#include "block.h"
#include "types.h"
#include "bitmap.h"
#include "free_list.h"
#include <stdio.h>
#include <limits.h>
#include "stack.h"
#include "heap.h"

#define UNW_LOCAL_ONLY

#include <libunwind.h>

typedef struct {
    int32_t id;
    word_t* name;
    int64_t size;
    int32_t dyn_method_count;
    word_t* dyn_method_salt;
    word_t* dyn_method_keys;
    word_t* dyn_methods;
    int64_t* ptr_map;
} Rtti;

extern word_t** __MODULES__;
extern int __MODULES_SIZE__;
extern int __OBJECT_ARRAY_ID__;

void mark_roots();
void mark_init(FreeList* free_list);

#endif // MARK_H
//
// Created by Lukas Kellenberger on 01.03.17.
//

#include <stdint.h>

//#define NDEBUG

#define TIMING_PRINT
//#define STACK_OVERFLOW_PRINT

#include <assert.h>

#ifndef UNTITLED_TYPES_H
#define UNTITLED_TYPES_H

typedef uint64_t word_t;

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

#endif //UNTITLED_TYPES_H

//
// Created by Lukas Kellenberger on 01.03.17.
//
#ifndef TYPES_H
#define TYPES_H

#define NDEBUG

#include <stdint.h>
#include <assert.h>


#define TIMING_PRINT
//#define STACK_OVERFLOW_PRINT


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

#endif //TYPES_H

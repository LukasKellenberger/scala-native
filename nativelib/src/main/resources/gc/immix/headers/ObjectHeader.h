//
// Created by Lukas Kellenberger on 19.04.17.
//

#ifndef IMMIX_OBJECTHEADER_H
#define IMMIX_OBJECTHEADER_H


#include <stdint.h>
#include <stdbool.h>
#include "gc_types.h"
#include "../Constants.h"

typedef enum {
    object_forward = 0x0,
    object_standard = 0x1,
    object_large = 0x2,
} ObjectType;

typedef struct {
    uint32_t size;
    uint8_t type;
    uint8_t marked;
} ObjectHeaderLine;

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

typedef word_t* Field_t;

typedef struct {
    ObjectHeaderLine header;
    Rtti* rtti;
    Field_t fields[0];
} ObjectHeader;

static inline bool object_isMarked(ObjectHeader* objectHeader) {
    return objectHeader->header.marked == 1;
}
static inline void object_markObjectHeader(ObjectHeader* objectHeader) {
    objectHeader->header.marked = 1;
}
static inline void object_unmarkObjectHeader(ObjectHeader* objectHeader) {
    objectHeader->header.marked = 0;
}

static inline bool object_isStandardObject(ObjectHeader* objectHeader) {
    return objectHeader->header.type == object_standard;
}
static inline bool object_isLargeObject(ObjectHeader* objectHeader) {
    return objectHeader->header.type == object_large;
}
static inline bool object_isForwardObject(ObjectHeader* objectHeader) {
    return objectHeader->header.type == object_forward;
}
static inline void object_setObjectType(ObjectHeader* objectHeader, ObjectType objectType) {
    objectHeader->header.type = objectType;
}

static inline size_t object_size(ObjectHeader* objectHeader) {
    uint32_t size = objectHeader->header.size;
    assert((object_isStandardObject(objectHeader) && size < LARGE_BLOCK_SIZE) || !object_isStandardObject(objectHeader));

    return size;
}

static inline void object_setSize(ObjectHeader* objectHeader, size_t size) {
    uint32_t _size = (uint32_t)size;
    assert(!object_isStandardObject(objectHeader) || (object_isStandardObject(objectHeader) && _size > 0 && _size < LARGE_BLOCK_SIZE));
    objectHeader->header.size = _size;
}



#endif //IMMIX_OBJECTHEADER_H

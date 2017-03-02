//
// Created by Lukas Kellenberger on 01.03.17.
//

#ifndef UNTITLED_BLOCK_H
#define UNTITLED_BLOCK_H

#include <stddef.h>
#include "types.h"

typedef enum {
    tag_allocated = 0,
    tag_free = 1
} tag_t;

#define BITS_FOR_TAG 8
#define TAG_MASK 0xFF

size_t header_unpack_size(word_t* block);

tag_t header_unpack_tag(word_t* block);

void header_pack_size(word_t* block, size_t size);

void header_pack_tag(word_t* block, tag_t tag);

void header_pack(word_t* block, size_t size, tag_t tag);

#endif //UNTITLED_BLOCK_H

//
// Created by Lukas Kellenberger on 01.03.17.
//

#include "block.h"


size_t header_unpack_size(word_t* block) {
    return (*block) >> BITS_FOR_TAG;
}

tag_t header_unpack_tag(word_t* block) {
    return *block & TAG_MASK;
}

void header_pack_size(word_t* block, size_t size) {
    *block = (*block & TAG_MASK) | (size << BITS_FOR_TAG);
}

void header_pack_tag(word_t* block, tag_t tag) {
    size_t size = header_unpack_size(block);
    header_pack(block, size, tag);
}

void header_pack(word_t* block, size_t size, tag_t tag) {
    *block = (size << 8) | (tag & TAG_MASK) ;
}
//
// Created by Lukas Kellenberger on 01.03.17.
//

#include "block.h"

Block* block_add_offset(Block* block, size_t offset) {
    return (Block*) ((word_t*)block + offset);
}

size_t header_unpack_size(word_t* block) {

    return ((Header*)block)->size;
}

tag_t header_unpack_tag(word_t* block) {
    return ((Header*)block)->tag;
}

void header_pack_size(word_t* block, size_t size) {
    Header* header = (Header*) block;
    header->size = size;
}

void header_pack_tag(word_t* block, tag_t tag) {
    Header* header = (Header*) block;
    header->tag = tag;
}

void header_pack(word_t* block, size_t size, tag_t tag) {
    Header* header = (Header*) block;
    header->size = size;
    header->tag = tag;
}
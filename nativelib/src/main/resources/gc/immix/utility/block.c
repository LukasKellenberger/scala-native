//
// Created by Lukas Kellenberger on 11.04.17.
//

#include <stdint.h>
#include "block.h"
#include "types.h"
#include "immix_constants.h"
#include "chunk.h"


void* block_align(void* ptr) {
    return (void*)((word_t)ptr & !BLOCK_MASK);
}

bool block_is_aligned(void* address) {
    return address == block_align(address);
}

int _get_chunk_index(void* block) {
    return (int)(((word_t) block & CHUNK_MASK) >> LOG_BYTES_IN_BLOCK);
}


/***************************************************************************
 * Block marking
 */

bool block_is_unused(void* address) {
    return block_get_block_mark_state(address) == UNALLOCATED_BLOCK_STATE;
}

bool block_is_unused_state(void* cursor) {
    return *(short_t*)cursor == UNALLOCATED_BLOCK_STATE;
}

short_t block_get_mark_state(void* cursor) {
    return *(short_t*)cursor;
}

void block_set_state(void* cursor, short_t value) {
    *(short_t*) cursor = value;
}

short_t block_get_block_mark_state(void* address) {
    return *(short_t*)block_get_block_mark_state_address(address);
}

void block_set_block_as_in_use(void* address) {
    block_set_block_state(address, UNMARKED_BLOCK_STATE);
}

void block_set_block_as_reused(void* address) {
    block_set_block_state(address, REUSED_BLOCK_STATE);
}

void block_set_block_as_unallocated(void* address) {
    *(short_t*)block_get_block_mark_state_address(address) = UNALLOCATED_BLOCK_STATE;
}

void block_set_block_state(void* address, short_t value) {
    *(short_t*)block_get_block_mark_state_address(address) = value;
}

void* block_get_block_mark_state_address(void* address) {
    void* chunk = chunk_align(address);
    int index = _get_chunk_index(address);
    return chunk + CHUNK_BLOCK_STATE_TABLE_OFFSET + (index << LOG_BYTES_IN_BLOCK_STATE_ENTRY);
}
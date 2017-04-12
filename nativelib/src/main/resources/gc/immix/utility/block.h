//
// Created by Lukas Kellenberger on 11.04.17.
//

#include <stdbool.h>
#include "types.h"

#ifndef IMMIX_BLOCK_H
#define IMMIX_BLOCK_H

#define UNALLOCATED_BLOCK_STATE 0
#define UNMARKED_BLOCK_STATE ((short_t) MAX_BLOCK_MARK_STATE + 1)
#define REUSED_BLOCK_STATE ((short_t) MAX_BLOCK_MARK_STATE + 2)

#define BLOCK_IS_NOT_DEFRAG_SOURCE 0
#define BLOCK_IS_DEFRAG_SOURCE 1

#define LOG_BYTES_IN_BLOCK_STATE_ENTRY LOG_BYTES_IN_SHORT
#define BYTES_IN_BLOCK_STATE_ENTRY (1 << LOG_BYTES_IN_BLOCK_STATE_ENTRY)
#define BLOCK_STATE_TABLE_BYTES (BLOCKS_IN_CHUNK << LOG_BYTES_IN_BLOCK_STATE_ENTRY)

#define LOG_BYTES_IN_BLOCK_DEFRAG_STATE_ENTRY LOG_BYTES_IN_SHORT;
#define BYTES_IN_BLOCK_DEFRAG_STATE_ENTRY (1 << LOG_BYTES_IN_BLOCK_DEFRAG_STATE_ENTRY)

#define BLOCK_DEFRAG_STATE_TABLE_BYTES (BLOCKS_IN_CHUNK << LOG_BYTES_IN_BLOCK_DEFRAG_STATE_ENTRY)

void* block_align(void* ptr);

bool block_is_aligned(void* address);

/***************************************************************************
 * Block marking
 */

bool block_is_unused(void* address);

bool block_is_unused_state(void* cursor);

short block_get_mark_state(void* cursor);

void block_set_state(void* cursor, short value);

short block_get_block_mark_state(void* address);

void block_set_block_as_in_use(void* address);

void block_set_block_as_reused(void* address);

void block_set_block_as_unallocated(void* address);

void block_set_block_state(void* address, short value);

void* block_get_block_mark_state_address(void* address);


#endif //IMMIX_BLOCK_H

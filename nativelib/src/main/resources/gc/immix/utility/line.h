//
// Created by Lukas Kellenberger on 11.04.17.
//

#ifndef IMMIX_LINE_H
#define IMMIX_LINE_H

#include "types.h"

#define LOG_BYTES_IN_LINE_STATUS 0
#define BYTES_IN_LINE_STATUS (1 << LOG_BYTES_IN_LINE_STATUS)

#define LINE_MARK_TABLE_BYTES (LINES_IN_CHUNK << LOG_BYTES_IN_LINE_STATUS)
#define LOG_LINE_MARK_BYTES_PER_BLOCK (LOG_LINES_IN_BLOCK + LOG_BYTES_IN_LINE_STATUS)
#define LINE_MARK_BYTES_PER_BLOCK (1 << LOG_LINE_MARK_BYTES_PER_BLOCK)

void* line_align(void* ptr);
bool line_is_aligned(void* address);
int line_get_chunk_index(void* line);

void line_mark(void* address, byte_t markValue);
void line_mark_multi_line(void* start, void* object, byte_t markValue);

void* line_get_chunk_mark_table(void* chunk);
void* line_get_block_mark_table(void* block);
int line_get_next_unavailable(void* baseLineAvailAddress, int line, byte_t unavailableState);
int line_get_next_available(void* baseLineAvailAddress, int line, byte_t unavailableState);

#endif //IMMIX_LINE_H

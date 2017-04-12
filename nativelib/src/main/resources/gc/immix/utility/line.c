//
// Created by Lukas Kellenberger on 11.04.17.
//

#include <stdlib.h>
#include "line.h"
#include "immix_constants.h"

void* _get_mark_address(void* address);
void* _get_meta_address(void* address, int tableOffset);


void* line_align(void* ptr) {
    return (void*)((word_t)ptr & !LINE_MASK);
}

bool line_is_aligned(void* address) {
    return address == line_align(address);
}

int line_get_chunk_index(void* line) {
    return (int)(((word_t)line & CHUNK_MASK) >> LOG_BYTES_IN_LINE);
}

void line_mark(void* address, byte_t markValue) {
    *(byte_t*)_get_mark_address(address) = markValue;
}
void line_mark_multi_line(void* start, void* object, byte_t markValue) {
    exit(1);
}


void* line_get_chunk_mark_table(void* chunk) {
    return _get_mark_address(chunk);
}

void* line_get_block_mark_table(void* block) {
    return _get_mark_address(block);
}

int line_get_next_unavailable(void* baseLineAvailAddress, int line, byte_t unavailableState) {
    while (line < LINES_IN_BLOCK &&
            *(byte_t*)(baseLineAvailAddress + INT_TO_WORD_EXTEND_ZEROS(line << LOG_BYTES_IN_LINE_STATUS)) < unavailableState)
        line++;
    return line;
}

int line_get_next_available(void* baseLineAvailAddress, int line, byte_t unavailableState) {
    byte_t last = *(byte_t*)(baseLineAvailAddress + INT_TO_WORD_EXTEND_ZEROS(line << LOG_BYTES_IN_LINE_STATUS));
    byte_t thisline;
    line++;
    while (line < LINES_IN_BLOCK) {
        thisline = *(byte_t*)(baseLineAvailAddress + INT_TO_WORD_EXTEND_ZEROS(line << LOG_BYTES_IN_LINE_STATUS));
        if (thisline < unavailableState && last < unavailableState)
            break;
        last = thisline;
        line++;
    }
    return line;
}


void* _get_mark_address(void* address) {
    return _get_meta_address(address, CHUNK_LINE_MARK_TABLE_OFFSET);
}

void* _get_meta_address(void* address, int tableOffset) {
    void* chunk = chunk_align(address);
    int index = line_get_chunk_index(address);
    return chunk + (tableOffset + (index << LOG_BYTES_IN_LINE_STATUS));
}
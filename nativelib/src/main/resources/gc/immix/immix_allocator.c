#include <memory.h>
#include "immix_allocator.h"
#include "utility/address_utils.h"
#include "utility/block.h"
#include "utility/immix_constants.h"
#include "utility/line.h"

void* _alloc_slow_hot(ImmixAllocator* immixAllocator, int byte, int align, int offset);
bool _acquire_recyclable_lines(ImmixAllocator* immixAllocator);
bool _acquire_recyclable_block(ImmixAllocator* immixAllocator);
bool _acquire_recyclable_block_address_order(ImmixAllocator* immixAllocator);

ImmixAllocator* immix_allocator_constructor(ImmixSpace* space, bool hot, bool copy) {
    ImmixAllocator* allocator = malloc(sizeof(ImmixAllocator));
    allocator->hot = hot;
    allocator->copy = copy;
    allocator->space = space;
    immix_allocator_reset(allocator);
}

void immix_allocator_reset(ImmixAllocator* immixAllocator) {
    immixAllocator->cursor = NULL;
    immixAllocator->limit = NULL;
    immixAllocator->largeCursor = NULL;
    immixAllocator->largeLimit = NULL;
    immixAllocator->markTable = NULL;
    immixAllocator->recyclableBlock = NULL;

    immixAllocator->requestForLarge = false;
    immixAllocator->recyclableExhausted = false;

    immixAllocator->line = LINES_IN_BLOCK;
    immixAllocator->lineUseCount = 0;
}

void* immix_allocator_alloc(ImmixAllocator* immixAllocator, int bytes, int align, int offset) {
    void* start = alignAllocationNoFill(immixAllocator->cursor, align, offset);
    void* end = start + bytes;

    if(end > immixAllocator->limit) {
        if(bytes > BYTES_IN_LINE) {
            return immix_allocator_overflow_alloc(immixAllocator, bytes, align, offset);
        } else {
            return _alloc_slow_hot(immixAllocator, bytes, align, offset);
        }
    }

    fill_alignement_gap(immixAllocator->cursor, start);
    immixAllocator->cursor = end;

    return start;
}

void* immix_allocator_overflow_alloc(ImmixAllocator* immixAllocator, int bytes, int align, int offset) {
    void* start = alignAllocationNoFill(immixAllocator->largeCursor, align, offset);
    void* end = start + bytes;


    if(end > immixAllocator->largeLimit) {
        immixAllocator->requestForLarge = true;
        void* rtn = alloc_slow_inline(bytes, align, offset);
        immixAllocator->requestForLarge = false;
        return rtn;
    }

    fill_alignement_gap(immixAllocator->largeCursor, start);
    immixAllocator->largeCursor = end;

    return start;

}

void* _alloc_slow_hot(ImmixAllocator* immixAllocator, int bytes, int align, int offset) {
    if(_acquire_recyclable_lines(immixAllocator)) {
        return immix_allocator_alloc(immixAllocator, bytes, align, offset);
    } else {
        return alloc_slow_inline(bytes, align, offset);
    }
}

bool _acquire_recyclable_lines(ImmixAllocator* immixAllocator) {
    while(immixAllocator->line < LINES_IN_BLOCK || _acquire_recyclable_block(immixAllocator)) {
        immixAllocator->line = immix_space_get_next_available_line(immixAllocator->space, immixAllocator->markTable, immixAllocator->line);
        if(immixAllocator->line < LINES_IN_BLOCK) {
            int endLine = immix_space_get_next_unavailable_line(immixAllocator->space, immixAllocator->markTable, immixAllocator->line);
            immixAllocator->cursor = immixAllocator->recyclableBlock + (immixAllocator->line << LOG_BYTES_IN_LINE);
            immixAllocator->limit = immixAllocator->recyclableBlock + (endLine << LOG_BYTES_IN_LINE);

            memset(immixAllocator->cursor, 0, immixAllocator->limit - immixAllocator->cursor);

            immixAllocator->line = endLine;

            return true;
        }
    }
    return false;
}

bool _acquire_recyclable_block(ImmixAllocator* immixAllocator) {
    bool rtn = _acquire_recyclable_block_address_order(immixAllocator);
    if(rtn) {
        immixAllocator->markTable = line_get_block_mark_table(immixAllocator->recyclableBlock);
        immixAllocator->line = 0;
    }

    return rtn;
}

bool _acquire_recyclable_block_address_order(ImmixAllocator* immixAllocator) {
    if(immixAllocator->recyclableExhausted) {
        return false;
    }

    int markState = 0;
    bool usable = false;
    while(!usable) {
        void* next = immixAllocator->recyclableBlock + BYTES_IN_BLOCK;
        if(immixAllocator->recyclableBlock == NULL || immix_space_is_recycle_alloc_chunk_aligned(next)) {
            immixAllocator->recyclableBlock = space_acquire_reusable_blocks();
            if(immixAllocator->recyclableBlock == NULL) {
                immixAllocator->recyclableExhausted = true;
                immixAllocator->line = LINES_IN_BLOCK;
                return false;
            }
        } else {
            immixAllocator->recyclableBlock = next;
        }
        markState = block_get_block_mark_state(immixAllocator->recyclableBlock);
        usable = (markState > 0 && markState <= immix_space_get_reusuable_mark_state_threshold(immixAllocator->copy));

        if(immixAllocator->copy && block_is_defrag_source(immixAllocator->recyclableBlock)) {
            usable = false;
        }
        block_set_block_as_reused(immixAllocator->recyclableBlock);

        immixAllocator->lineUseCount += (LINES_IN_BLOCK - markState);
        return true;
    }
}

// ALLOCATOR


void* allocator_alloc_slow_inline(ImmixAllocator* immixAllocator, int bytes, int alignment, int offset) {
    ImmixSpace* space = immixAllocator->space;

    bool emergencyCollection = false;
    while(true) {
        void* result = immix_allocator_alloc_slow_once(immixAllocator, bytes, alignment, offset);

        if(result != NULL) {
            if(!immixAllocator->allocationSuccess) {

            }
        }
    }
}

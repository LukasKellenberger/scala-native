//
// Created by Lukas Kellenberger on 19.04.17.
//

#ifndef IMMIX_BLOCKHEADER_H
#define IMMIX_BLOCKHEADER_H


#include <stdint.h>
#include "LineHeader.h"
#include "gc_types.h"
#include "../Constants.h"
#include "ObjectHeader.h"

typedef enum {
    block_free = 0x0,
    block_recyclable = 0x1,
    block_unavailable = 0x2
} BlockFlag;

typedef word_t Line[WORDS_IN_LINE];

typedef struct {
    struct {
        uint8_t mark;
        uint8_t flags;
        int16_t first;
        int32_t nextBlock;
    } header;
    LineHeader lineHeaders[LINE_COUNT];
    Line lines[LINE_COUNT];
} BlockHeader;

static inline bool block_isRecyclable(BlockHeader* blockHeader) {
    return blockHeader->header.flags == block_recyclable;
}
static inline bool block_isUnavailable(BlockHeader* blockHeader) {
    return blockHeader->header.flags == block_unavailable;
}
static inline bool block_isFree(BlockHeader* blockHeader) {
    return blockHeader->header.flags == block_free;
}
static inline void block_setFlag(BlockHeader* blockHeader, BlockFlag blockFlag) {
    blockHeader->header.flags = blockFlag;
}

static inline bool block_isMarked(BlockHeader* blockHeader) {
    return blockHeader->header.mark == 1;
}

static inline void block_unmark(BlockHeader* blockHeader) {
    blockHeader->header.mark = 0;
}

static inline void block_mark(BlockHeader* blockHeader) {
    blockHeader->header.mark = 1;
}

static inline BlockHeader* block_getBlockHeader(word_t* word) {
    return (BlockHeader*)((word_t)word & BLOCK_SIZE_IN_BYTES_INVERSE_MASK);

}

static inline FreeLineHeader* block_getFreeLineHeader(BlockHeader* blockHeader, int lineIndex) {
    return (FreeLineHeader*)&blockHeader->lines[lineIndex][0];
}

static inline BlockHeader* block_blockHeaderFromObjectHeader(ObjectHeader* objectHeader) {
    return block_getBlockHeader((word_t*) objectHeader);
}

static inline BlockHeader* block_blockHeaderFromLineHeader(LineHeader* lineHeader) {
    return block_getBlockHeader((word_t*) lineHeader);
}

static inline word_t* block_getFirstWord(BlockHeader* blockHeader) {
    return (word_t*)(&blockHeader->lines);
}


static inline word_t* block_getBlockEnd(BlockHeader* blockHeader) {
    return block_getFirstWord(blockHeader) + (WORDS_IN_LINE * LINE_COUNT);
}

static inline uint32_t block_getLineIndexFromLineHeader(BlockHeader* blockHeader, LineHeader* lineHeader) {
    return (uint32_t) (lineHeader - blockHeader->lineHeaders);
}

static inline uint32_t block_getLineIndexFromWord(BlockHeader* blockHeader, word_t* word) {
    word_t* firstWord = block_getFirstWord(blockHeader);
    return (uint32_t)((word_t)word - (word_t)firstWord) >> LINE_SIZE_BITS;
}

static inline uint32_t block_getLineIndexFromObjectHeader(BlockHeader* blockHeader, ObjectHeader* objectHeader) {
    return block_getLineIndexFromWord(blockHeader, (word_t*)objectHeader);
}


#endif //IMMIX_BLOCKHEADER_H

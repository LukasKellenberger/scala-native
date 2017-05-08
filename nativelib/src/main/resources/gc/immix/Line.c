#include <stdio.h>
#include "Line.h"
#include "headers/BlockHeader.h"


ObjectHeader* line_header_getFirstObject(LineHeader* lineHeader) {
    BlockHeader* blockHeader = block_blockHeaderFromLineHeader(lineHeader);
    uint8_t offset = line_header_getFirstObjectOffset(lineHeader);
    uint8_t wordOffset = offset / WORD_SIZE;
    uint32_t lineIndex = block_getLineIndexFromLineHeader(blockHeader, lineHeader);
    return (ObjectHeader*)(&blockHeader->lines[lineIndex][wordOffset]);

}

inline void line_header_update(word_t* objectStart) {
    BlockHeader* blockHeader = block_getBlockHeader(objectStart);
    int lineIndex = block_getLineIndexFromWord(blockHeader, objectStart);
    LineHeader* lineHeader = &blockHeader->lineHeaders[lineIndex];

    size_t wordDiff = objectStart - &(blockHeader->lines[lineIndex][0]);
    uint8_t offset = (uint8_t)(wordDiff << WORD_SIZE_BITS);
    if(!line_header_containsObject(lineHeader)) {
        line_header_setOffset(lineHeader, offset);
    }
}



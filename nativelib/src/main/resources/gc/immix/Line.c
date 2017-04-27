//
// Created by Lukas Kellenberger on 19.04.17.
//

#include <printf.h>
#include "Line.h"
#include "headers/BlockHeader.h"


ObjectHeader* line_header_getFirstObject(LineHeader* lineHeader) {
    BlockHeader* blockHeader = block_blockHeaderFromLineHeader(lineHeader);
    uint8_t offset = line_header_getFirstObjectOffset(lineHeader);
    uint8_t wordOffset = offset / WORD_SIZE;
    uint32_t lineIndex = block_getLineIndexFromLineHeader(blockHeader, lineHeader);
    return (ObjectHeader*)(&blockHeader->lines[lineIndex][wordOffset]);

}

void line_header_setFirstObject(LineHeader* lineHeader, word_t* objectHeader) {

    BlockHeader* blockHeader = block_blockHeaderFromLineHeader(lineHeader);
    uint32_t lineIndex = block_getLineIndexFromLineHeader(blockHeader, lineHeader);

    size_t wordDiff = objectHeader - &(blockHeader->lines[lineIndex][0]);

    uint8_t offset = (uint8_t)(wordDiff << WORD_SIZE_BITS);

    if(!line_header_containsObject(lineHeader) || line_header_getFirstObjectOffset(lineHeader) > offset) {
        //printf("update offset %d %d %p!\n", lineIndex, offset, objectHeader);
        line_header_setOffset(lineHeader, offset);
    }
}

void line_header_update(word_t* objectStart, word_t* objectEnd) {
    BlockHeader* blockHeader = block_getBlockHeader(objectStart);
    int lineIndex = block_getLineIndexFromWord(blockHeader, objectStart);
    LineHeader* lineHeader = &blockHeader->lineHeaders[lineIndex];

    line_header_setLineContainsObject(lineHeader);

    lineIndex = block_getLineIndexFromWord(blockHeader, objectEnd);
    lineHeader = &blockHeader->lineHeaders[lineIndex];

    size_t wordDiff = objectEnd - &(blockHeader->lines[lineIndex][0]);

    uint8_t offset = (uint8_t)(wordDiff << WORD_SIZE_BITS);

    if(!line_header_containsObject(lineHeader) || line_header_getFirstObjectOffset(lineHeader) > offset) {
        //printf("update offset %d %d %p!\n", lineIndex, offset, objectEnd);
        line_header_setOffset(lineHeader, offset);
    }
}

void line_header_setContainsObject(word_t* objectStart) {
    BlockHeader* blockHeader = block_getBlockHeader(objectStart);
    int lineIndex = block_getLineIndexFromWord(blockHeader, objectStart);
    LineHeader* lineHeader = &blockHeader->lineHeaders[lineIndex];
    line_header_setLineContainsObject(lineHeader);
}

void line_header_setObjectEnd(word_t* objectEnd) {
    BlockHeader* blockHeader = block_getBlockHeader(objectEnd);
    int lineIndex = block_getLineIndexFromWord(blockHeader, objectEnd);
    LineHeader* lineHeader = &blockHeader->lineHeaders[lineIndex];

    size_t wordDiff = objectEnd - &(blockHeader->lines[lineIndex][0]);

    uint8_t offset = (uint8_t)(wordDiff << WORD_SIZE_BITS);

    if(!line_header_containsObject(lineHeader) || line_header_getFirstObjectOffset(lineHeader) > offset) {
        //printf("update offset %d %d %p!\n", lineIndex, offset, objectEnd);
        line_header_setOffset(lineHeader, offset);
    }
}
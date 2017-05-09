#include <stdio.h>
#include <memory.h>
#include "Block.h"
#include "Object.h"
#include "Log.h"

bool block_lineContainsObject(BlockHeader* blockHeader, int lineIndex) {
    LineHeader* lineHeader = &(blockHeader->lineHeaders[lineIndex]);
    return line_header_containsObject(lineHeader);
}

ObjectHeader* block_lineGetFirstObject(BlockHeader* blockHeader, int lineIndex) {
    LineHeader* lineHeader = &(blockHeader->lineHeaders[lineIndex]);
    return line_header_getFirstObject(lineHeader);
}

void block_recycle(Allocator* allocator, BlockHeader* blockHeader) {

    if(!block_isMarked(blockHeader)) {
        memset(blockHeader, 0, BLOCK_TOTAL_SIZE);
        blockList_addLast(&allocator->freeBlocks, blockHeader);
        allocator->freeBlockCount++;
    } else {
        assert(block_isMarked(blockHeader));
        block_unmark(blockHeader);
        int16_t lineIndex = 0;
        int lastRecyclable = -1;
        while(lineIndex < LINE_COUNT) {
            LineHeader* lineHeader = &blockHeader->lineHeaders[lineIndex];
            if(line_header_isMarked(lineHeader)) {
                // Unmark line
                line_header_unmark(lineHeader);
                // If the line contains an object
                if(line_header_containsObject(lineHeader)) {
                    //Unmark all objects in line
                    ObjectHeader *object = line_header_getFirstObject(lineHeader);
                    word_t *lineEnd = block_getLineAddress(blockHeader, lineIndex) + LINE_SIZE / WORD_SIZE;
                    while (object != NULL && (word_t *) object < lineEnd) {
                        object_unmarkObjectHeader(object);
                        object = object_nextObject(object);
                    }
                }
                lineIndex++;
            } else {
                if(lastRecyclable == -1) {
                    blockHeader->header.first = lineIndex;
                } else {
                    block_getFreeLineHeader(blockHeader, lastRecyclable)->next = lineIndex;
                }
                lastRecyclable = lineIndex;
                line_header_setEmpty(lineHeader);
                lineIndex++;
                uint8_t size = 1;
                while(lineIndex < LINE_COUNT && !line_header_isMarked(lineHeader = &blockHeader->lineHeaders[lineIndex])) {
                    size++;
                    lineIndex++;
                    line_header_setEmpty(lineHeader);
                }
                block_getFreeLineHeader(blockHeader, lastRecyclable)->size = size;
                memset(block_getLineAddress(blockHeader, lastRecyclable), 0, size*LINE_SIZE);
            }
        }
        if(lastRecyclable == -1) {
            block_setFlag(blockHeader, block_unavailable);
        } else {
            block_getFreeLineHeader(blockHeader, lastRecyclable)->next = LAST_HOLE;
            block_setFlag(blockHeader, block_recyclable);
            blockList_addLast(&allocator->recycledBlocks, blockHeader);
            allocator->recyclableBlockCount++;
        }
    }
}
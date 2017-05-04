//
// Created by Lukas Kellenberger on 19.04.17.
//

#include <stdio.h>
#include <memory.h>
#include "Block.h"
#include "Object.h"
#include "headers/BlockHeader.h"
#include "headers/LineHeader.h"
#include "Allocator.h"

bool block_lineContainsObject(BlockHeader* blockHeader, int lineIndex) {
    LineHeader* lineHeader = &(blockHeader->lineHeaders[lineIndex]);
    return line_header_containsObject(lineHeader);
}

ObjectHeader* block_lineGetFirstObject(BlockHeader* blockHeader, int lineIndex) {
    LineHeader* lineHeader = &(blockHeader->lineHeaders[lineIndex]);
    return line_header_getFirstObject(lineHeader);
}

void block_recycle(Allocator* allocator, BlockHeader* blockHeader) {
    /*for(int i=0; i < LINE_COUNT; i++) {
        if(line_header_isMarked(&blockHeader->lineHeaders[i])) {
            printf("M");
        } else {
            printf("A");
        }
    }
    printf("\n");*/

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
                    word_t *lineEnd = &blockHeader->lines[lineIndex + 1][0];
                    while (object != NULL && (word_t *) object < lineEnd) {
                        object_unmarkObjectHeader(object);
                        object = object_next_object(object);
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
                memset(&blockHeader->lines[lastRecyclable][0], 0, size*LINE_SIZE);
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
    /*block_print(blockHeader);
    for(int i=0; i < LINE_COUNT; i++) {
        if(line_header_containsObject(&blockHeader->lineHeaders[i])) {
            printf("C");
        } else {
            printf("E");
        }
    }
    printf("\n");*/
}

void block_print(BlockHeader* blockHeader) {
    if(block_isFree(blockHeader)){
        printf("Block %p, FREE\n", blockHeader);
    } else if(block_isRecyclable(blockHeader)) {
        printf("Block %p, RECYCLABLE: ", blockHeader);
        int lineIndex = blockHeader->header.first;
        while(lineIndex != LAST_HOLE) {
            FreeLineHeader* lineHeader = block_getFreeLineHeader(blockHeader, lineIndex);
            printf("[%d size: %u] -> ", lineIndex, lineHeader->size);
            lineIndex = block_getFreeLineHeader(blockHeader, lineIndex)->next;
        }
        printf("\n");
    } else {
        printf("Block %p, UNAVAILABLE\n", blockHeader);
    }
}
//
// Created by Lukas Kellenberger on 22.04.17.
//

#include <stddef.h>
#include <printf.h>
#include "BlocList.h"




int32_t _getBlockIndex(word_t* heapStart, BlockHeader* blockHeader) {
    return (uint32_t)((word_t*)blockHeader - heapStart) / WORDS_IN_BLOCK;
}

BlockHeader* _getBlockFromIndex(word_t* heapStart, int32_t index) {
    return (BlockHeader*)(heapStart + (index * WORDS_IN_BLOCK));
}

BlockHeader* _getNextBlock(word_t* heapStart, BlockHeader* header) {
    int32_t nextBlockId = header->header.nextBlock;
    if(nextBlockId == LAST_BLOCK) {
        return NULL;
    } else if(nextBlockId == 0) {
        nextBlockId = _getBlockIndex(heapStart, header) + 1;
    }
    return _getBlockFromIndex(heapStart, nextBlockId);
}


void blockList_init(BlockList* blockList, word_t* heapStart){
    blockList->heapStart= heapStart;
    blockList->first = NULL;
    blockList->last = NULL;
}

inline bool blockList_isEmpty(BlockList* blockList) {
    return blockList->first == NULL;
}

BlockHeader* blockList_removeFirstBlock(BlockList* blockList) {
    assert(blockList->first != NULL);
    BlockHeader* block = blockList->first;
    if(block == blockList->last) {
        blockList->first = NULL;
    }
    blockList->first = _getNextBlock(blockList->heapStart, block);
    return block;
}

void blockList_addLast(BlockList* blockList, BlockHeader* blockHeader) {
    if(blockList->first == NULL) {
        blockList->first = blockHeader;
    } else {
        blockList->last->header.nextBlock = _getBlockIndex(blockList->heapStart, blockHeader);
    }
    blockList->last = blockHeader;
    blockHeader->header.nextBlock = LAST_BLOCK;
}

void blockList_clear(BlockList* blockList) {
    blockList->first = NULL;
    blockList->last = NULL;
}

void blockList_print(BlockList* blockList) {
    printf("BlockList: ");
    BlockHeader* current = blockList->first;
    while(current != NULL) {
        printf("[%p] -> ", current);
        current = _getNextBlock(blockList->heapStart, current);
    }
    printf("\n");

}

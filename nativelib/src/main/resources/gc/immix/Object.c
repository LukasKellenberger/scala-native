//
// Created by Lukas Kellenberger on 19.04.17.
//

#include <stddef.h>
#include <printf.h>
#include "Object.h"
#include "headers/BlockHeader.h"
#include "Line.h"


ObjectHeader* objectNextLargeObject(ObjectHeader* objectHeader) {
    size_t size = object_chunkSize(objectHeader);
    assert(size != 0);
    return (ObjectHeader*)((ubyte_t *)objectHeader + size);
}

ObjectHeader* object_next_object(ObjectHeader* objectHeader) {
    size_t size = object_size(objectHeader);
    assert(size < LARGE_BLOCK_SIZE);
    if(size == 0) {
        return NULL;
    }
    ObjectHeader* next = (ObjectHeader*)((ubyte_t *)objectHeader + size);
    assert(block_getBlockHeader((word_t*)next) == block_getBlockHeader((word_t*)objectHeader));
    return next;
}

static inline bool isWordAligned(word_t* word) {
    return ((word_t)word & WORD_MASK) == (word_t)word;
}

ObjectHeader* object_getObject(word_t* word) {
    if(!isWordAligned(word)) {
        printf("Could be inner pointer (Not aligned) %p\n", word);
        return NULL;
    }
    BlockHeader* blockHeader = block_getBlockHeader(word);
    if(word < block_getFirstWord(blockHeader)) {
        printf("Points on block header\n");
        return NULL;
    }
    uint32_t lineIndex = block_getLineIndexFromWord(blockHeader, word);
    if(!line_header_containsObject(&blockHeader->lineHeaders[lineIndex])) {
        printf("Empty line\n");
        return NULL;
    }

    ObjectHeader* current = line_header_getFirstObject(&blockHeader->lineHeaders[lineIndex]);
    while(current != NULL && word >= (word_t*)object_next_object(current)) {
        current = object_next_object(current);
    }
    if((word_t*)current == word) {
        return current;
    } else {
        printf("Could be inner pointer %p\n", word);
        return NULL;
    }
}

ObjectHeader* object_getLargeObject(LargeAllocator* allocator, word_t* word) {
    if(bitmap_getBit(allocator->bitmap, (ubyte_t*) word)) {
        printf("Mark large!\n");
        return (ObjectHeader*) word;
    } else {
        printf("Could be inner pointer %p (Large)\n", word);
        return NULL;
    }
}

void object_mark(ObjectHeader* objectHeader) {
    // Mark the object itself
    object_markObjectHeader(objectHeader);

    if(!object_isLargeObject(objectHeader)) {
        // Mark the block
        BlockHeader *blockHeader = block_getBlockHeader((word_t *) objectHeader);
        block_mark(blockHeader);

        // Mark all Lines
        int startIndex = block_getLineIndexFromObjectHeader(blockHeader, objectHeader);
        word_t* lastWord = (word_t*)object_next_object(objectHeader) - 1;
        int endIndex = block_getLineIndexFromWord(blockHeader, lastWord);
        assert(startIndex >= 0 && startIndex < LINE_COUNT);
        assert(endIndex >= 0 && endIndex < LINE_COUNT);
        assert(startIndex <= endIndex);
        for (int i = startIndex; i <= endIndex; i++) {
            LineHeader *lineHeader = &blockHeader->lineHeaders[i];
            line_header_mark(lineHeader);
        }
    }

}

size_t object_chunkSize(ObjectHeader* objectHeader) {
    return (object_size(objectHeader) + MIN_BLOCK_SIZE - 1) / MIN_BLOCK_SIZE * MIN_BLOCK_SIZE;
}


bool object_isInnerPointerOrNext(ObjectHeader* objectHeader, word_t* ptr) {
    return false;
}
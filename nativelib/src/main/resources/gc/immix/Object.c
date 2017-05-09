#include <stddef.h>
#include <stdio.h>
#include "Object.h"
#include "headers/BlockHeader.h"
#include "Line.h"
#include "Log.h"

ObjectHeader* objectNextLargeObject(ObjectHeader* objectHeader) {
    size_t size = object_chunkSize(objectHeader);
    assert(size != 0);
    return (ObjectHeader*)((ubyte_t *)objectHeader + size);
}

ObjectHeader* object_nextObject(ObjectHeader *objectHeader) {
    size_t size = object_size(objectHeader);
    assert(size < LARGE_BLOCK_SIZE);
    if(size == 0) {
        return NULL;
    }
    ObjectHeader* next = (ObjectHeader*)((ubyte_t *)objectHeader + size);
    assert(block_getBlockHeader((word_t*)next) == block_getBlockHeader((word_t*)objectHeader) || (ubyte_t*)block_getBlockHeader((word_t*)next) == (ubyte_t*)block_getBlockHeader((word_t*)objectHeader) + BLOCK_TOTAL_SIZE);
    return next;
}

static inline bool isWordAligned(word_t* word) {
    return ((word_t)word & WORD_MASK) == (word_t)word;
}

ObjectHeader* object_getFromInnerPointerInLine(BlockHeader* blockHeader, int lineIndex, word_t* innerPointer) {
    ObjectHeader* object = line_header_getFirstObject(&blockHeader->lineHeaders[lineIndex]);
    while(object != NULL && (word_t*) object_nextObject(object) <= innerPointer) {
        object = object_nextObject(object);
    }
    if(object == NULL) {
        return NULL;
    } else {
        ObjectHeader* nextObjectStart = object_nextObject(object);
        if(object < nextObjectStart) {
            return object;
        } else {
            return NULL;
        }
    }
}

ObjectHeader* object_getFromInnerPointer(word_t* word) {
    assert(isWordAligned(word));
    BlockHeader* blockHeader = block_getBlockHeader(word);
    uint32_t lineIndex = block_getLineIndexFromWord(blockHeader, word);
    ObjectHeader* header = NULL;
    if(line_header_containsObject(&blockHeader->lineHeaders[lineIndex]) && (word_t*)line_header_getFirstObject(&blockHeader->lineHeaders[lineIndex]) <= word) {
        // Search in line
        header = object_getFromInnerPointerInLine(blockHeader, lineIndex, word);
    } else {
        // Search in previous lines
        bool contains = false;
        lineIndex--;
        while(lineIndex > 0 && !(contains = line_header_containsObject(&blockHeader->lineHeaders[lineIndex]))) {
            lineIndex--;
        }
        assert(lineIndex < LINE_COUNT);
        if(contains) {
            header = object_getFromInnerPointerInLine(blockHeader, lineIndex, word);
        }
    }
    if(!(header == NULL || (word >= (word_t*) header && word < (word_t*) object_nextObject(header)))) {
        printf("header: %p %p %p\n", header, word, (word_t*) object_nextObject(header));
        fflush(stdout);
    }
    assert(header == NULL || (word >= (word_t*) header && word < (word_t*) object_nextObject(header)));
    return header;
}

ObjectHeader* object_getObject(word_t* word) {
    BlockHeader* blockHeader = block_getBlockHeader(word);
    if(word < block_getFirstWord(blockHeader)) {
        printf("Points on block header\n");
        return NULL;
    }
    if(!isWordAligned(word)) {
        //printf("Could be inner pointer (Not aligned) %p\n", word);
        return object_getFromInnerPointer((word_t*)((word_t) word & WORD_MASK));
    }
    uint32_t lineIndex = block_getLineIndexFromWord(blockHeader, word);
    if(!line_header_containsObject(&blockHeader->lineHeaders[lineIndex])) {
        printf("Empty line\n");
        return NULL;
    }

    ObjectHeader* current = line_header_getFirstObject(&blockHeader->lineHeaders[lineIndex]);
    while(current != NULL && word >= (word_t*) object_nextObject(current)) {
        current = object_nextObject(current);
    }
    if((word_t*)current == word) {
        return current;
    } else {
        //printf("Could be inner pointer %p\n", word);
        return object_getFromInnerPointer(word);
    }
}

ObjectHeader* object_getLargeInnerPointer(LargeAllocator* allocator, word_t* word) {
    word_t* current = (word_t*)((word_t)word & LARGE_BLOCK_MASK);

    while(!bitmap_getBit(allocator->bitmap, (ubyte_t*)current)) {
        current -= LARGE_BLOCK_SIZE/WORD_SIZE;
    }

    ObjectHeader* objectHeader = (ObjectHeader*) current;
    if(word < (word_t*)objectHeader + object_chunkSize(objectHeader)/WORD_SIZE && objectHeader->rtti != NULL) {
        return objectHeader;
    } else {

        return NULL;
    }
}

ObjectHeader* object_getLargeObject(LargeAllocator* allocator, word_t* word) {
    if(bitmap_getBit(allocator->bitmap, (ubyte_t*) word)) {
        return (ObjectHeader*) word;
    } else {
        printf("Could be inner pointer %p (Large)\n", word);
        fflush(stdout);
        ObjectHeader* object = object_getLargeInnerPointer(allocator, word);
        assert(object == NULL || (word >= (word_t*) object && word < (word_t*) objectNextLargeObject(object)));
        return object;
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
        word_t* lastWord = (word_t*) object_nextObject(objectHeader) - 1;
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

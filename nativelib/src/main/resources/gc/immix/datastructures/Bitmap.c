#include <stdio.h>
#include <string.h>
#include "Bitmap.h"
#include "../Constants.h"
#include "../Log.h"

// size to address in bytes, nbBits = size / granularity
Bitmap* bitmap_alloc(size_t size, word_t* offset) {

    size_t nbBits = size / BITMAP_GRANULARITY;
    // Number of words needed to store `nbBits` bits
    unsigned long nbWords = (nbBits + BITS_PER_WORD - 1) / BITS_PER_WORD;
    void* words = calloc(nbWords, sizeof(word_t));
    Bitmap* bitmap = malloc(sizeof(Bitmap));
    bitmap->words = words;
    bitmap->size = size;
    bitmap->offset = (ubyte_t*)offset;
    return bitmap;
}

size_t addressToIndex(ubyte_t* offset, ubyte_t* addr) {
    return (addr - offset) / BITMAP_GRANULARITY;
}

void bitmap_setBit(Bitmap* bitmap, ubyte_t* addr) {
    assert(addr >= bitmap->offset && addr < bitmap->offset + bitmap->size);
    size_t index = addressToIndex(bitmap->offset, addr);
    bitmap->words[WORD_OFFSET(index)] |= (1LLU << BIT_OFFSET(index));
}

void bitmap_clearBit(Bitmap* bitmap, ubyte_t* addr) {
    assert(addr >= bitmap->offset && addr < bitmap->offset + bitmap->size);

    size_t index = addressToIndex(bitmap->offset, addr);

    bitmap->words[WORD_OFFSET(index)] &= ~(1LLU << BIT_OFFSET(index));
}

int bitmap_getBit(Bitmap* bitmap, ubyte_t* addr) {
    assert(addr >= bitmap->offset && addr < bitmap->offset + bitmap->size);

    size_t index = addressToIndex(bitmap->offset, addr);
    word_t bit = bitmap->words[WORD_OFFSET(index)] & (1LLU << BIT_OFFSET(index));
    return bit != 0;
}

void bitmap_print(Bitmap* bitmap) {
    ubyte_t * current = bitmap->offset;
    while (current < bitmap->offset + bitmap->size) {
        if(bitmap_getBit(bitmap, current)) {
            printf("%p ", current);
            fflush(stdout);
        }
        current += BITMAP_GRANULARITY;
    }
    printf("\n");
}

// Increment in bytes
void bitmap_grow(Bitmap* bitmap, size_t increment) {
    bitmap_print(bitmap);
    size_t nbBits = bitmap->size / BITMAP_GRANULARITY;
    size_t nbBitsIncrement = increment / BITMAP_GRANULARITY;

    size_t previousNbWords = (nbBits + BITS_PER_WORD - 1) / BITS_PER_WORD;

    size_t totalNbWords = (nbBits + nbBitsIncrement + BITS_PER_WORD - 1) / BITS_PER_WORD;

    bitmap->words = realloc(bitmap->words, totalNbWords * WORD_SIZE);
    bitmap->size += increment;

    memset(bitmap->words + previousNbWords, 0, (totalNbWords - previousNbWords) * WORD_SIZE);

    bitmap_print(bitmap);
}

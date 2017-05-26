#ifndef IMMIX_BITMAP_H
#define IMMIX_BITMAP_H

#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "../GCTypes.h"

typedef struct {
    size_t size;
    word_t *words;
    ubyte_t *offset;
} Bitmap;

#define BITS_PER_WORD (sizeof(word_t) * CHAR_BIT)
#define WORD_OFFSET(b) (b / BITS_PER_WORD)
#define BIT_OFFSET(b) (b % BITS_PER_WORD)

#define BITMAP_GRANULARITY MIN_BLOCK_SIZE

Bitmap *Bitmap_alloc(size_t size, word_t *offset);

void Bitmap_setBit(Bitmap *bitmap, ubyte_t *addr);

void Bitmap_clearBit(Bitmap *bitmap, ubyte_t *addr);

int Bitmap_getBit(Bitmap *bitmap, ubyte_t *addr);

void Bitmap_grow(Bitmap *bitmap, size_t nb_words);

#endif // IMMIX_BITMAP_H

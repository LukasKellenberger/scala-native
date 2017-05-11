#ifndef IMMIX_LINEHEADER_H
#define IMMIX_LINEHEADER_H


#include <stdint.h>
#include <stdbool.h>

#define FIRST_OBJECT_OFFSET_MASK (uint8_t)0xFC


typedef struct {
    int16_t next;
    uint16_t size;
} FreeLineHeader;

typedef enum {
    line_empty = 0x0,
    line_marked = 0x1,
    line_contains_object = 0x2,
} LineFlag;

/*
 * Contains the offset of bytes to the first object.
 * The size of a line is 256 bytes, thus 8 bits are enough to store the offset.
 *
 * The offset is word aligned, meaning that the 3 lower bits can be used of flags.
 * Bit 0 is for marking
 * Bit 1 is for indicating if the line contains an object
 *
 */
typedef uint8_t LineHeader;

static inline bool line_header_isMarked(LineHeader* lineHeader) {
    return (line_marked & *lineHeader) != 0;
}
static inline void line_header_mark(LineHeader* lineHeader) {
    *lineHeader |= line_marked;
}
static inline void line_header_unmark(LineHeader* lineHeader) {
    *lineHeader &= ~line_marked;
}

static inline void line_header_setEmpty(LineHeader* lineHeader) {
    *lineHeader = (uint8_t)line_empty;
}

static inline bool line_header_containsObject(LineHeader* lineHeader) {
    return (line_contains_object & *lineHeader) != 0;
}

static inline void line_header_setOffset(LineHeader* lineHeader, uint8_t offset) {
    *lineHeader = (offset & FIRST_OBJECT_OFFSET_MASK);
}

static inline void line_header_setLineContainsObject(LineHeader* lineHeader) {
    *lineHeader |= line_contains_object;
}


static inline uint8_t line_header_getFirstObjectOffset(LineHeader* lineHeader) {
    return *lineHeader & FIRST_OBJECT_OFFSET_MASK;
}

#endif //IMMIX_LINEHEADER_H

#ifndef IMMIX_MARKER_H
#define IMMIX_MARKER_H

#include "Heap.h"
#include "datastructures/Stack.h"

void marker_mark(Heap* heap, Stack*);
void marker_markField(Heap* heap, Stack* stack, ObjectHeader* object, int index);
void marker_markObjectArray(Heap* heap, Stack* stack, ObjectHeader* object);
void mark_roots(Heap* heap, Stack* stack);

#endif //IMMIX_MARKER_H

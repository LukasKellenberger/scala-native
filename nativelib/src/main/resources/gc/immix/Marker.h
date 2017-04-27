//
// Created by Lukas Kellenberger on 21.04.17.
//

#ifndef IMMIX_MARKER_H
#define IMMIX_MARKER_H

#include "ProgramRoots.h"
#include "Heap.h"
#include "Stack.h"

void marker_markRoots(Heap*, Roots*, Stack*);
void marker_mark(Heap* heap, Stack*);
void mark_roots(Heap* heap, Stack* stack);

#endif //IMMIX_MARKER_H

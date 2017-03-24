#ifndef HEAP_SANITY_H
#define HEAP_SANITY_H

#include "free_list.h"
#include "linked_list.h"
#include "bitmap.h"
#include "block.h"
#include "types.h"
#include "heap.h"

void heap_sanity_full_check(Heap* heap);

#endif //HEAP_SANITY_H
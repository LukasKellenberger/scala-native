#ifndef IMMIX_ALLOCATORSTATS_H
#define IMMIX_ALLOCATORSTATS_H

#include <stdint.h>

typedef struct {
    uint64_t totalBytesAllocated;
    uint64_t bytesAllocated;
    uint64_t liveObjectCount;
    uint64_t totalAllocatedObjectCount;
} AllocatorStats;

AllocatorStats* allocatorStats_create();
void allocatorStats_reset(AllocatorStats* stats);
void allocatorStats_print(AllocatorStats* stats);



#endif //IMMIX_ALLOCATORSTATS_H

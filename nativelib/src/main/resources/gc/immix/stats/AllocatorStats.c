#include <stdio.h>
#include <stdlib.h>
#include "AllocatorStats.h"

AllocatorStats* allocatorStats_create() {
    AllocatorStats* stats = malloc(sizeof(AllocatorStats));
    allocatorStats_reset(stats);
    return stats;
}

void allocatorStats_reset(AllocatorStats* stats) {
    stats->totalBytesAllocated = 0;
    stats->bytesAllocated = 0;
    stats->liveObjectCount = 0;
    stats->totalAllocatedObjectCount = 0;
}

void allocatorStats_print(AllocatorStats* stats) {
    printf("############\n");
    printf("Total bytes allocated: %llu\n", stats->totalBytesAllocated);
    printf("Bytes allocated since last collect: %llu\n", stats->bytesAllocated);
    printf("Live object count: %llu\n", stats->liveObjectCount);
    printf("Total objects allocated: %llu\n", stats->totalAllocatedObjectCount);
    printf("############\n");
    fflush(stdout);
}
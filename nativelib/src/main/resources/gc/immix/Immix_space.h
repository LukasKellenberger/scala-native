//
// Created by Lukas Kellenberger on 11.04.17.
//

#ifndef IMMIX_IMMIXSPACE_H
#define IMMIX_IMMIXSPACE_H


#include "utility/types.h"

short_t reusableMarkStateThreshold;

typedef struct {
    byte_t markState;
    byte_t lineMarkState;
    byte_t lineUnavailState;
    int linesConsumed;
    void* allocBlockCursor;
    void* allocSentinel;
    bool exhaustedReusableSpace;
    ChunkList* chunkList;
    Defrag* defrag;
} ImmixSpace;

int immix_space_get_next_unavailable_line(ImmixSpace* immixSpace, void* baseLineAvailAddress, int line);
int immix_space_get_next_available_line(ImmixSpace* immixSpace, void* baseLineAvailAddress, int line);

#endif //IMMIX_IMMIXSPACE_H

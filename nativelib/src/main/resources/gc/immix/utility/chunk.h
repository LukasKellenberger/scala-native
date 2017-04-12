//
// Created by Lukas Kellenberger on 11.04.17.
//

#ifndef IMMIX_CHUNK_H
#define IMMIX_CHUNK_H

#include "types.h"

void* chunk_align(void* ptr);

bool chunk_is_aligned(void* address);

int chunk_get_bytes_offset(void* address);


#endif //IMMIX_CHUNK_H

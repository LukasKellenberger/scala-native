#ifndef FREE_LIST_STATS_H
#define FREE_LIST_STATS_H

#include "free_list.h"
#include <stdio.h>

typedef struct {
    size_t list_counts[LINKED_LIST_NUMBER];
    size_t largest_word;
    size_t total;
} MemoryStats;

void free_list_print_stats(FreeList* list);

#endif //FREE_LIST_STATS_H
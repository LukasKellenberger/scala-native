#ifndef FREE_LIST_UTILS_H
#define FREE_LIST_UTILS_H

#include <stdio.h>
#include "../free_list.h"
#include "linked_list_utils.h"

typedef struct {
    size_t list_counts[LINKED_LIST_NUMBER];
    size_t largest_word;
    size_t total;
} MemoryStats;

void free_list_print_stats(FreeList* list);
void free_list_print(FreeList* free_list);

#endif //FREE_LIST_UTILS_H
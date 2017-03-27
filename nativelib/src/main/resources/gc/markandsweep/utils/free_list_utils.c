#include "free_list_utils.h"

MemoryStats free_list_get_stats(FreeList* free_list) {
    MemoryStats stats;
    size_t total = 0;
    size_t largest_word = 0;
    for(int i=0; i < LINKED_LIST_NUMBER; i++) {
        LinkedList* list = free_list->list[i];
        Block* current = list->first;
        long count = 0;
        while(current != NULL) {
            count++;
            size_t size = current->header.size;
            if(size > largest_word) {
                largest_word = size;
            }
            total += size;
            current = current->next;
        }
        stats.list_counts[i] = count;
    }



    stats.largest_word = largest_word;
    stats.total = total;

    return stats;
}

void free_list_print_stats(FreeList* list) {
    MemoryStats stats = free_list_get_stats(list);

    printf("### STATS ###\n");
    printf("counts: [");
    for(int i=0; i < LINKED_LIST_NUMBER - 1; i++) {
        printf("%zu, ", stats.list_counts[i]);
    }
    printf("%zu]\n", stats.list_counts[LINKED_LIST_NUMBER - 1]);
    printf("total free: %zu / %zu\n", stats.total, list->size / sizeof(word_t));
    printf("max: %zu\n", stats.largest_word);
    printf("#############\n");
}


void free_list_print(FreeList* list) {
    for(int i=0; i < LINKED_LIST_NUMBER; i++) {
        if(list->list[i]->first != NULL) {
            printf("%d: ", i + SMALLEST_BLOCK_SIZE);
            linked_list_print(list->list[i]);
            linked_list_check(list->list[i], i < 14 ? i + 2 : -1, list->bitmap);
        }
    }
    printf("\n");
}


//
// Created by Lukas Kellenberger on 01.03.17.
//

#include <printf.h>
#include <assert.h>
#include "free_list.h"
#include "linked_list.h"


FreeList* free_list_create(size_t size) {
    FreeList* free_list = malloc(sizeof(FreeList));
    LinkedList* list = linked_list_alloc(size);
    free_list->list = list;
    free_list->bitmap = bitmap_alloc(size, list->start);

    bitmap_set_bit(free_list->bitmap, list->first);

    return free_list;
}

word_t* free_list_add_block(FreeList* list, word_t* block, word_t* previous_block) {
    word_t* added_block = linked_list_add_block(list->list, block, previous_block);
    if(added_block == previous_block) {
        bitmap_clear_bit(list->bitmap, block);
    }
    return added_block;
}

/*
 * size without header in bytes
 */
word_t* free_list_get_block(FreeList* list, size_t size) {
    BestMatch best_match = linked_list_find_block(list->list, size);
    if(best_match.block == NULL) {
        return NULL;
    }
    word_t* block = best_match.block;
    word_t* previous = best_match.previous;
    size_t block_size_with_header = header_unpack_size(block) + 1;

    //Block matches size (if size +1 cannot split)
    if(block_size_with_header > size/sizeof(word_t) + 2) {
        //Split the block
        linked_list_split_block(list->list, block, size);
        bitmap_set_bit(list->bitmap, block + size/ sizeof(word_t) + 1);
    }
    bitmap_set_bit(list->bitmap, block);
    linked_list_remove_block(list->list, block, previous);

    return block;

}

word_t* free_list_next(FreeList* free_list, word_t* node) {
    return linked_list_next(free_list->list, node);
}

void free_list_print(FreeList* list) {
    linked_list_print(list->list);
}

MemoryStats free_list_get_stats(FreeList* list) {
    LinkedList* linked_list = list->list;
    word_t* current = linked_list->first;
    size_t total = 0;
    size_t max = 0;
    long count = 0;
    while(current != NULL) {
        count++;
        size_t size = header_unpack_size(current) + 1;
        if(size > max) {
            max = size;
        }
        total += size;
        current = linked_list_next(linked_list, current);
    }
    MemoryStats stats;
    stats.available_words = total;
    stats.largest_word = max;
    stats.nb_words = count;

    return stats;
}

void free_list_print_stats(FreeList* list) {
    MemoryStats stats = free_list_get_stats(list);

    printf("### STATS ###\n");
    printf("total free: %zu / %zu\n", stats.available_words, list->list->size / sizeof(word_t));
    printf("max: %zu\n", stats.largest_word);
    printf("nb: %ld\n", stats.nb_words);
    printf("#############\n");
}

void print_memory(FreeList* free_list) {
    word_t* current = free_list->list->start;

    word_t* end = free_list->list->start + (free_list->list->size / sizeof(word_t));

    while(current < end) {
        assert(bitmap_get_bit(free_list->bitmap, current));
        size_t size = header_unpack_size(current) + 1;
        if(header_unpack_tag(current) == tag_allocated) {
            printf("|A %zu", size);
        } else if(header_unpack_tag(current) == tag_free) {
            printf("|F %zu", size);
        }
        size_t current_size_h = header_unpack_size(current) + 1;
        current += current_size_h;
    }

    printf("\n");

}
//
// Created by Lukas Kellenberger on 01.03.17.
//

#include "free_list.h"
#include <stdio.h>


inline static int size_to_linked_list(size_t nb_words) {
    if(nb_words <= SMALLEST_BLOCK_SIZE) {
        return 0;
    } else {
        int log_floor = log2_floor(nb_words) - 1;
        return log_floor > LINKED_LIST_NUMBER - 1 ? LINKED_LIST_NUMBER - 1 : log_floor;
    }
}



FreeList* free_list_create(size_t nb_words, word_t* heap_start, Bitmap* bitmap) {
    FreeList* free_list = malloc(sizeof(FreeList));

    word_t* words = heap_start;

    free_list->bitmap = bitmap;
    free_list->size = nb_words * sizeof(word_t);
    free_list->start = words;


    for(int i=0; i < LINKED_LIST_NUMBER; i++) {
        free_list->list[i] = linked_list_alloc();
    }

    linked_list_add_block(free_list->list[size_to_linked_list(nb_words)], (Block*) words, nb_words);
    bitmap_set_bit(free_list->bitmap, words);
    free_list->free = nb_words;

    return free_list;
}

void free_list_add_block(FreeList* list, word_t* block, size_t total_block_size) {

    assert(total_block_size % 2 == 0);
    list->free += total_block_size;
    size_t remaining_size = total_block_size;
    word_t* current = block;
    while(remaining_size > 0) {
        int log2_f = log2_floor(remaining_size);
        size_t block_size = 1L << log2_f;
        int list_index = size_to_linked_list(block_size);
        linked_list_add_block(list->list[list_index], (Block*) current, block_size);
        bitmap_set_bit(list->bitmap, current);
        current += block_size;
        remaining_size -= block_size;
    }

}

word_t* free_list_get_block(FreeList* list, size_t object_size) {
    size_t block_size = 1L << log2_ceil(object_size);

    int list_index = size_to_linked_list(block_size);
    Block* block = NULL;

    while(list_index <= LINKED_LIST_NUMBER - 1 && (block = list->list[list_index]->first) == NULL) {
        ++list_index;
    }
    if(block == NULL) {
        return NULL;
    }

    size_t block_size_with_header = block->header.size;

    if(block_size_with_header - SMALLEST_BLOCK_SIZE >= block_size) {
        Block* remaining_block = block_add_offset(block, block_size);
        bitmap_set_bit(list->bitmap, (word_t*)remaining_block);
        linked_list_remove_block(list->list[list_index], block, object_size, NULL);
        size_t remaining_block_size_with_header = block_size_with_header - block_size;
        free_list_add_block(list, (word_t*)remaining_block, remaining_block_size_with_header);
    } else {
        linked_list_remove_block(list->list[list_index], block, object_size, NULL);
    }

    bitmap_set_bit(list->bitmap, (word_t*) block);
    assert(block == NULL || header_unpack_size((word_t*)block) >= object_size);
    list->free -= block_size;

    return (word_t*)block;

}

void free_list_clear_lists(FreeList* list) {
    list->free = 0;
    for(int i = 0; i < LINKED_LIST_NUMBER; i++) {
        list->list[i]->first = NULL;
    }
}

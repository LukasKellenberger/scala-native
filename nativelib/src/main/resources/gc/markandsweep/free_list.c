//
// Created by Lukas Kellenberger on 01.03.17.
//

#include "free_list.h"
#include <stdio.h>

inline static int log2_floor(size_t v) {
    static const int MultiplyDeBruijnBitPosition[32] =
            {
                    0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30,
                    8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31
            };

    v |= v >> 1; // first round down to one less than a power of 2
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;

    return MultiplyDeBruijnBitPosition[(uint32_t)(v * 0x07C4ACDDU) >> 27];
}

int log2_ceil(size_t v) {
    return log2_floor(2*v - 1);
}


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

void free_list_add_block(FreeList* list, word_t* block, size_t block_size_with_header) {

    assert(block_size_with_header % 2 == 0);
    list->free += block_size_with_header;
    //printf("size: %zu\n", block_size_with_header);
    size_t remaining_size = block_size_with_header;
    word_t* current = block;
    while(remaining_size > 0) {
        int log2_f = log2_floor(remaining_size);
        size_t block_size = 1L << log2_f;
        int list_index = size_to_linked_list(block_size);
        //printf("list index %d\n", list_index);
        linked_list_add_block(list->list[list_index], (Block*) current, block_size);
        bitmap_set_bit(list->bitmap, current);
        current += block_size;
        remaining_size -= block_size;
        //printf("rem size: %zu\n", remaining_size);
        //fflush(stdout);
    }
    //printf("end add\n");
    //fflush(stdout);
}

/*
 * size without header in bytes
 */

Block* get_block_last_list(FreeList* list, size_t nb_words_with_header) {
    int nb_words = nb_words_with_header - 1;

    BestMatch best_match = linked_list_find_block(list->list[LINKED_LIST_NUMBER - 1], nb_words_with_header);
    if(best_match.block == NULL) {
        return NULL;
    }
    Block* block = best_match.block;
    Block* previous = best_match.previous;
    size_t block_size_with_header = block->header.size;

    //Block matches size (if size +1 cannot split)
    if(block_size_with_header - SMALLEST_BLOCK_SIZE >= nb_words_with_header) {

        Block* remaining_block = block_add_offset(block, nb_words_with_header);
        bitmap_set_bit(list->bitmap, (word_t*)remaining_block);
        linked_list_remove_block(list->list[LINKED_LIST_NUMBER - 1], block, nb_words_with_header, previous);
        size_t remaining_block_size_with_header = block_size_with_header - nb_words_with_header;
        free_list_add_block(list, (word_t*)remaining_block, remaining_block_size_with_header);

    } else {
        linked_list_remove_block(list->list[LINKED_LIST_NUMBER - 1], block, block_size_with_header, previous);
    }

    return block;
}

word_t* free_list_get_block(FreeList* list, size_t nb_words_with_header) {
    size_t block_size = 1L << log2_ceil(nb_words_with_header);

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
        linked_list_remove_block(list->list[list_index], block, block_size, NULL);
        size_t remaining_block_size_with_header = block_size_with_header - block_size;
        free_list_add_block(list, (word_t*)remaining_block, remaining_block_size_with_header);
    } else {
        linked_list_remove_block(list->list[list_index], block, block_size_with_header, NULL);
    }

    bitmap_set_bit(list->bitmap, (word_t*) block);
    assert(block == NULL || header_unpack_size((word_t*)block) >= nb_words_with_header);
    list->free -= block_size;

    return (word_t*)block;

}

void free_list_clear_lists(FreeList* list) {
    list->free = 0;
    for(int i = 0; i < LINKED_LIST_NUMBER; i++) {
        list->list[i]->first = NULL;
    }
}

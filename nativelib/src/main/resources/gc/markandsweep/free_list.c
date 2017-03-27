//
// Created by Lukas Kellenberger on 01.03.17.
//

#include "free_list.h"

inline static int log2(size_t v) {
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


inline static int size_to_linked_list(size_t nb_words, int add) {
    if(nb_words <= SMALLEST_BLOCK_SIZE) {
        return 0;
    } else if(nb_words <= MAX_CONST_SIZE_LIST) {
        return nb_words - SMALLEST_BLOCK_SIZE;
    } else {
        int size_log2 = log2(nb_words);
        if(size_log2 < LINKED_LIST_NUMBER - MAX_CONST_SIZE_LIST + SMALLEST_BLOCK_SIZE - 1 + POWER_MAX_CONST_SIZE) {
            int index = MAX_CONST_SIZE_LIST - SMALLEST_BLOCK_SIZE + size_log2 - POWER_MAX_CONST_SIZE;
            return add ? index : index + 1;
        } else {
            return LINKED_LIST_NUMBER - 1;
        }
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

    linked_list_add_block(free_list->list[size_to_linked_list(nb_words, 1)], (Block*) words, nb_words);
    bitmap_set_bit(free_list->bitmap, words);

    return free_list;
}

void free_list_add_block(FreeList* list, word_t* block, size_t block_size_with_header) {

    const int list_index = size_to_linked_list(block_size_with_header, 1);
    linked_list_add_block(list->list[list_index], (Block*) block, block_size_with_header);
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
    size_t block_size_with_header = block->header.size + 1;

    //Block matches size (if size +1 cannot split)
    if(block_size_with_header - SMALLEST_BLOCK_SIZE >= nb_words_with_header) {

        Block* remaining_block = block_add_offset(block, nb_words_with_header);
        bitmap_set_bit(list->bitmap, (word_t*)remaining_block);
        linked_list_remove_block(list->list[LINKED_LIST_NUMBER - 1], block, nb_words_with_header, previous);
        size_t remaining_block_size_with_header = block_size_with_header - nb_words_with_header;
        linked_list_add_block(list->list[size_to_linked_list(remaining_block_size_with_header, 1)], remaining_block, remaining_block_size_with_header);

    } else {
        linked_list_remove_block(list->list[LINKED_LIST_NUMBER - 1], block, block_size_with_header, previous);
    }

    return block;
}

word_t* free_list_get_block(FreeList* list, size_t nb_words_with_header) {
    int size = nb_words_with_header * sizeof(word_t);
    int list_index = size_to_linked_list(nb_words_with_header, 0);
    Block* block = NULL;
    if(list_index == LINKED_LIST_NUMBER - 1) {
        block = get_block_last_list(list, nb_words_with_header);
        if(block == NULL) {
            return NULL;
        } else {
            bitmap_set_bit(list->bitmap, (word_t*) block);
            return (word_t*)block;
        }
    } else {
        while(list_index < LINKED_LIST_NUMBER - 1 && (block = list->list[list_index]->first) == NULL) {
            ++list_index;
        }
        if(list_index == LINKED_LIST_NUMBER - 1) {
            block = get_block_last_list(list, nb_words_with_header);
            if(block == NULL) {
                return NULL;
            } else {
                bitmap_set_bit(list->bitmap, (word_t*) block);
                assert(block == NULL || header_unpack_size((word_t*)block) >= nb_words_with_header - 1);
                return (word_t*)block;
            }
        } else {
            size_t block_size_with_header = block->header.size + 1;

            if(block_size_with_header - SMALLEST_BLOCK_SIZE >= nb_words_with_header) {
                Block* remaining_block = block_add_offset(block, nb_words_with_header);
                bitmap_set_bit(list->bitmap, (word_t*)remaining_block);
                linked_list_remove_block(list->list[list_index], block, nb_words_with_header, NULL);
                size_t remaining_block_size_with_header = block_size_with_header - nb_words_with_header;
                linked_list_add_block(list->list[size_to_linked_list(remaining_block_size_with_header, 1)], remaining_block, remaining_block_size_with_header);
            } else {
                linked_list_remove_block(list->list[list_index], block, block_size_with_header, NULL);
            }

            bitmap_set_bit(list->bitmap, (word_t*) block);
            assert(block == NULL || header_unpack_size((word_t*)block) >= nb_words_with_header - 1);
            return (word_t*)block;
        }
    }
}

void free_list_clear_lists(FreeList* list) {
    for(int i = 0; i < LINKED_LIST_NUMBER; i++) {
        list->list[i]->first = NULL;
    }
}

//
// Created by Lukas Kellenberger on 01.03.17.
//

#include <printf.h>
#include "free_list.h"
#include "linked_list.h"

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

// size in words
inline static int size_to_linked_list(size_t size, int add) {
    if(size <= SMALLEST_BLOCK_SIZE) {
        return 0;
    } else if(size <= MAX_CONST_SIZE_LIST) {
        return size - SMALLEST_BLOCK_SIZE;
    } else {
        int size_log2 = log2(size);
        if(size_log2 < LINKED_LIST_NUMBER - MAX_CONST_SIZE_LIST + SMALLEST_BLOCK_SIZE - 1 + POWER_MAX_CONST_SIZE) {
            int index = MAX_CONST_SIZE_LIST - SMALLEST_BLOCK_SIZE + size_log2 - POWER_MAX_CONST_SIZE;
            return add ? index : index + 1;
        } else {
            return LINKED_LIST_NUMBER - 1;
        }
    }
    //return 0;
}

FreeList* free_list_create(size_t size, word_t* heap_start, Bitmap* bitmap) {
    FreeList* free_list = malloc(sizeof(FreeList));

    size_t nb_words = size / sizeof(word_t);

    word_t* words = heap_start;

    free_list->bitmap = bitmap;
    free_list->size = size;
    free_list->start = words;


    for(int i=0; i < LINKED_LIST_NUMBER; i++) {
        free_list->list[i] = linked_list_create();
    }

    linked_list_add_block(free_list->list[size_to_linked_list(nb_words, 1)], (Block*) words, nb_words);
    bitmap_set_bit(free_list->bitmap, words);

    return free_list;
}


void free_list_add_block(FreeList* list, word_t* block, size_t block_size) {
    // block-size is without header, size_to_linked_list with header
    assert(block_size > 1);
    const int list_index = size_to_linked_list(block_size, 1);
    linked_list_add_block(list->list[list_index], (Block*) block, block_size);
}

/*
 * size with header in words
 */

Block* get_block_last_list(FreeList* list, size_t size_to_alloc) {
    assert(size_to_alloc > 1);

    BestMatch best_match = linked_list_find_block(list->list[LINKED_LIST_NUMBER - 1], size_to_alloc);
    if(best_match.block == NULL) {
        return NULL;
    }
    Block* block = best_match.block;
    Block* previous = best_match.previous;
    size_t total_block_size = block->header.size;
    assert(total_block_size > 1);

    //If we can create at least a Min_size block: split
    if(total_block_size >= size_to_alloc + SMALLEST_BLOCK_SIZE) {
        //Split the block
        /*linked_list_split_block(list->list[LINKED_LIST_NUMBER - 1], block, size_to_alloc);
        Block* remaining_block = block_add_offset(block, size_to_alloc);
        bitmap_set_bit(list->bitmap, (word_t*)remaining_block);

        size_t remaining_block_size = total_block_size - size_to_alloc;
        assert(remaining_block_size > 1);

        int list_index = size_to_linked_list(remaining_block_size, 0);
        // remove the block we want to alloc from the list
        linked_list_remove_block(list->list[LINKED_LIST_NUMBER - 1], block, size_to_alloc, previous);

        // If the remaining block is not large enough to stay in it's current list, remove and re-add it
        if(list_index < LINKED_LIST_NUMBER - 1) {

            linked_list_remove_block(list->list[LINKED_LIST_NUMBER - 1], remaining_block, remaining_block_size, previous);
            linked_list_add_block(list->list[list_index], remaining_block, remaining_block_size);
        }*/

        Block* remaining_block = block_add_offset(block, size_to_alloc);
        bitmap_set_bit(list->bitmap, (word_t*)remaining_block);
        linked_list_remove_block(list->list[LINKED_LIST_NUMBER - 1], block, size_to_alloc, previous);
        size_t remaining_block_size = total_block_size - size_to_alloc;
        linked_list_add_block(list->list[size_to_linked_list(remaining_block_size, 1)], remaining_block, remaining_block_size);


    } else {
        // If we don't split, just remove the block
        linked_list_remove_block(list->list[LINKED_LIST_NUMBER - 1], block, total_block_size, previous);
    }

    return block;
}

word_t* free_list_get_block(FreeList* list, size_t size_to_alloc) {
    // get the smallest index with blocks large enough
    int list_index = size_to_linked_list(size_to_alloc, 0);
    Block* block = NULL;
    // If we get the last list, use special method to get block from there
    if(list_index == LINKED_LIST_NUMBER - 1) {
        block = get_block_last_list(list, size_to_alloc);
        if(block == NULL) {
            return NULL;
        } else {
            bitmap_set_bit(list->bitmap, (word_t*) block);
            assert(block == NULL || header_unpack_size((word_t*)block) >= size_to_alloc);
            return (word_t*)block;
        }
    } else {
        // If there is no block at index, increment it until finding one
        while(list_index < LINKED_LIST_NUMBER - 1 && (block = list->list[list_index]->first) == NULL) {
            ++list_index;
        }
        // Again if index is the last list, use special method
        if(list_index == LINKED_LIST_NUMBER - 1) {
            block = get_block_last_list(list, size_to_alloc);
            if(block == NULL) {
                return NULL;
            } else {
                bitmap_set_bit(list->bitmap, (word_t*) block);
                assert(block == NULL || header_unpack_size((word_t*)block) >= size_to_alloc);
                return (word_t*)block;
            }

        } else {
            size_t total_block_size = block->header.size;

            if(total_block_size >= size_to_alloc + SMALLEST_BLOCK_SIZE) {
                Block* remaining_block = block_add_offset(block, size_to_alloc);
                bitmap_set_bit(list->bitmap, (word_t*)remaining_block);
                linked_list_remove_block(list->list[list_index], block, size_to_alloc, NULL);
                size_t remaining_block_size = total_block_size - size_to_alloc;
                linked_list_add_block(list->list[size_to_linked_list(remaining_block_size, 1)], remaining_block, remaining_block_size);
            } else {
                linked_list_remove_block(list->list[list_index], block, total_block_size, NULL);
            }

            bitmap_set_bit(list->bitmap, (word_t*) block);
            assert(block == NULL || header_unpack_size((word_t*)block) >= size_to_alloc);
            return (word_t*)block;
        }
    }
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
void free_list_check(FreeList* list) {
    for(int i=0; i < LINKED_LIST_NUMBER; i++) {
        if(list->list[i]->first != NULL) {
            linked_list_check(list->list[i], i < 14 ? i + 2 : -1, list->bitmap);
        }
    }
}

void free_list_clear_lists(FreeList* list) {
    for(int i=0; i < LINKED_LIST_NUMBER; i++) {
        list->list[i]->first = NULL;
    }
}

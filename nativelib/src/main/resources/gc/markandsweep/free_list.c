//
// Created by Lukas Kellenberger on 01.03.17.
//

#include <printf.h>
#include <assert.h>
#include "free_list.h"
#include "linked_list.h"


inline static int size_to_linked_list(size_t size) {
    if(size <= SMALLEST_BLOCK_SIZE) {
        return 0;
    } else if(size >= LINKED_LIST_NUMBER + SMALLEST_BLOCK_SIZE - 1) {
        return LINKED_LIST_NUMBER - 1;
    } else {
        return size - SMALLEST_BLOCK_SIZE;
    }
}

FreeList* free_list_create(size_t size) {
    FreeList* free_list = malloc(sizeof(FreeList));

    unsigned long nb_words = size / sizeof(word_t);
    word_t* words = calloc(nb_words, sizeof(word_t));

    free_list->bitmap = bitmap_alloc(size, words);
    free_list->size = size;
    free_list->start = words;


    for(int i=0; i < LINKED_LIST_NUMBER; i++) {
        free_list->list[i] = linked_list_create(size);
    }

    linked_list_add_block(free_list->list[size_to_linked_list(nb_words)], words, nb_words - 1);
    bitmap_set_bit(free_list->bitmap, words);

    return free_list;
}

void free_list_add_block(FreeList* list, word_t* block, size_t block_size) {
    // block-size is without header, size_to_linked_list with header
    const int list_index = size_to_linked_list(block_size + 1);
    linked_list_add_block(list->list[list_index], block, block_size);
}

/*
 * size without header in bytes
 */

word_t* get_block_last_list(FreeList* list, size_t size) {
    int nb_words = size / sizeof(word_t);

    BestMatch best_match = linked_list_find_block(list->list[LINKED_LIST_NUMBER - 1], size);
    if(best_match.block == NULL) {
        return NULL;
    }
    word_t* block = best_match.block;
    word_t* previous = best_match.previous;
    size_t block_size_with_header = header_unpack_size(block) + 1;

    //Block matches size (if size +1 cannot split)
    if(block_size_with_header > nb_words + SMALLEST_BLOCK_SIZE) {
        //Split the block
        linked_list_split_block(list->list[LINKED_LIST_NUMBER - 1], block, size);
        word_t* remaining_block = block + nb_words + 1;
        bitmap_set_bit(list->bitmap, remaining_block);

        size_t remaining_block_size_with_header = block_size_with_header - (nb_words + 1);
        int list_index = size_to_linked_list(remaining_block_size_with_header);
        linked_list_remove_block(list->list[LINKED_LIST_NUMBER - 1], block, nb_words, previous);

        if(list_index < LINKED_LIST_NUMBER - 1) {
            //printf("need to move block !!! %lu\n", s);

            linked_list_remove_block(list->list[LINKED_LIST_NUMBER - 1], remaining_block, remaining_block_size_with_header - 1, previous);
            linked_list_add_block(list->list[list_index], remaining_block, remaining_block_size_with_header - 1);
        }

    } else {
        linked_list_remove_block(list->list[LINKED_LIST_NUMBER - 1], block, block_size_with_header - 1, previous);
    }
    //check_list(list, list->list[LINKED_LIST_NUMBER - 1]);

    return block;
}

/*word_t* free_list_get_block(FreeList* list, size_t size) {
    int list_index = size_to_linked_list(size + 1);
    word_t* block = NULL;

    if(list_index == LINKED_LIST_NUMBER -1) {
        block = get_block_last_list(list, size);
        if(block == NULL) {
            return NULL;
        }
    } else {

        //Exact size
        block = list->list[list_index]->first;
        if(block != NULL) {
            linked_list_remove_block(list->list[list_index], block, size, NULL);
        } else {
            ++list_index;
            while(list_index < LINKED_LIST_NUMBER - 2 && list->list[list_index]->first == NULL) {
                ++list_index;
            }
            block = list->list[list_index]->first;
            if(list_index >= size + 1 + SMALLEST_BLOCK_SIZE) {
                printf("Need to split block !!! %lu\n", list_index - (size + 1));
            }
            // Found a block before last list
            // TODO: Split if possible
            if(block != NULL) {
                linked_list_remove_block(list->list[list_index], block, list_index - 1, NULL);
            } else {
                block = get_block_last_list(list, size);
                if(block == NULL) {
                    return NULL;
                }
            }
        }
    }
    bitmap_set_bit(list->bitmap, block);

    return block;
}*/

word_t* free_list_get_block(FreeList* list, size_t size) {
    int nb_words = size / sizeof(word_t);
    int list_index = size_to_linked_list(nb_words + 1);
    word_t* block = NULL;
    if(list_index == LINKED_LIST_NUMBER - 1) {
        block = get_block_last_list(list, size);
        if(block == NULL) {
            return NULL;
        } else {
            bitmap_set_bit(list->bitmap, block);
            return block;
        }
    } else {
        while(list_index < LINKED_LIST_NUMBER - 1 && (block = (word_t*)(list->list[list_index]->first)) == NULL) {
            ++list_index;
        }

        if(list_index == LINKED_LIST_NUMBER - 1) {
            block = get_block_last_list(list, size);
            if(block == NULL) {
                return NULL;
            } else {
                bitmap_set_bit(list->bitmap, block);
                return block;
            }
        } else {
            if(list_index >= nb_words + 1 + SMALLEST_BLOCK_SIZE) {
                //free_list_print(list);
                linked_list_remove_block(list->list[list_index], block, nb_words, NULL);
                word_t* remaining_block = block + nb_words + 1;
                size_t remaining_block_size_with_header = list_index + SMALLEST_BLOCK_SIZE - (nb_words + 1);
                linked_list_add_block(list->list[size_to_linked_list(remaining_block_size_with_header)], remaining_block, remaining_block_size_with_header - 1);
                bitmap_set_bit(list->bitmap, remaining_block);
                //free_list_print(list);

            } else {
                linked_list_remove_block(list->list[list_index], block, list_index + SMALLEST_BLOCK_SIZE - 1, NULL);
            }
            //check_list(list, list->list[list_index]);

            bitmap_set_bit(list->bitmap, block);
            return block;
        }
    }
}


void free_list_print(FreeList* list) {
    for(int i=0; i < LINKED_LIST_NUMBER; i++) {
        printf("%d: ", i + SMALLEST_BLOCK_SIZE);
        linked_list_print(list->list[i]);
    }
}

void free_list_clear_lists(FreeList* list) {
    for(int i=0; i < LINKED_LIST_NUMBER; i++) {
        list->list[i]->first = NULL;
    }
}

/*MemoryStats free_list_get_stats(FreeList* list) {
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

}*/
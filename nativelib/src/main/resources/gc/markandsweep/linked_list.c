//
// Created by Lukas Kellenberger on 01.03.17.
//

#include <assert.h>
#include <stdlib.h>
#include <printf.h>
#include "linked_list.h"


word_t addr_p_to_v(LinkedList* list, word_t* p_addr) {
    word_t diff = p_addr - list->start;
    return diff;
}

word_t* addr_v_to_p(LinkedList* list, word_t addr) {
    return list->start + addr;
}

/*
 * size in bytes
 */
LinkedList* linked_list_alloc(size_t size) {
    assert(size % sizeof(word_t) == 0);
    unsigned long nb_words = size / sizeof(word_t);
    assert(nb_words > 1);
    word_t* words = calloc(nb_words, sizeof(word_t));
    LinkedList* linked_list = malloc(sizeof(LinkedList));
    linked_list->first = words;
    linked_list->start = words;
    linked_list->size = size;

    header_pack(words, nb_words - 1, tag_free);

    return linked_list;
}

word_t* linked_list_add_block(LinkedList* list, word_t* block, word_t* previous) {
    size_t block_size = header_unpack_size(block);

    header_pack(block, block_size, tag_free);
    assert(block_size >= 1);


    if(list->first == NULL) {
        list->first = block;
        block[1] = LIST_END;

    } else if(previous == NULL) {
        word_t next = addr_p_to_v(list, list->first);
        list->first = block;
        block[1] = next;
    } else {
        size_t previous_size = header_unpack_size(previous);

        // If previous and current are contiguous, merge them
        if(previous + previous_size + 1 == block) {
            header_pack(previous, previous_size + block_size + 1, tag_free);
            previous[1] = LIST_END;
            block = previous;
        } else {
            previous[1] = addr_p_to_v(list, block);
            block[1] = LIST_END;
        }
    }

    return block;
}
void linked_list_remove_block(LinkedList* list, word_t* block, word_t* previous) {
    assert(block != NULL);
    word_t next = block[1];

    if (list->first == block) {
        // If the block is the first element of the list
        list->first = next == LIST_END ? NULL : addr_v_to_p(list, next);
    } else {
        previous[1] = next;
    }

    header_pack_tag(block, tag_allocated);
}
word_t* linked_list_next(LinkedList* list, word_t* block) {
    assert(block != NULL);

    if(block == NULL) {
        return NULL;
    }
    word_t next = block[1];
    if(next == LIST_END) {
        return NULL;
    }
    return addr_v_to_p(list, next);
}
/*
 * size without header in bytes
 */
void linked_list_split_block(LinkedList* list, word_t* block, size_t size) {
    assert(size % sizeof(word_t) == 0);
    size_t size_with_header = size / sizeof(word_t) + 1;
    // minimal block size is 2
    assert(size_with_header > 1);
    size_t block_size_with_header = header_unpack_size(block) + 1;


    assert(size_with_header + 2 <= block_size_with_header);
    //remaining block size must be at least 2
    size_t remaining_size_with_header = block_size_with_header - size_with_header;
    assert(remaining_size_with_header > 1);
    word_t next = block[1];

    word_t* remaining_free_block = block + size_with_header;

    header_pack(block, size_with_header - 1, tag_free);
    block[1] = addr_p_to_v(list, remaining_free_block);

    header_pack(remaining_free_block, remaining_size_with_header - 1, tag_free);
    remaining_free_block[1] = next;
}

void linked_list_print(LinkedList* list) {
    word_t* current = list->first;
    printf("list: ");
    while(current != NULL) {
        size_t size = header_unpack_size(current) + 1;
        printf("[%p (%zu)] -> ", current, size);
        current = linked_list_next(list, current);
    }
    printf("\n");
}

/*
 * size without header in bytes
 */
BestMatch linked_list_find_block(LinkedList* list, size_t size) {
    assert(size % sizeof(word_t) == 0);
    size_t nb_words_with_header = size / sizeof(word_t) + 1;

    word_t* previous_best = NULL;
    word_t* best = NULL;
    size_t best_size = SIZE_MAX;
    word_t* previous = NULL;
    word_t* current = list->first;
    while(current != NULL) {
        size_t current_size_with_header = header_unpack_size(current) + 1;
        if(current_size_with_header == nb_words_with_header) {
            best = current;
            previous_best = previous;
            break;
        } else if(current_size_with_header > nb_words_with_header && current_size_with_header < best_size) {
            best = current;
            best_size = current_size_with_header;
            previous_best = previous;
        }
        previous = current;
        current = linked_list_next(list, current);
    }


    BestMatch bestMatch;
    bestMatch.previous = previous_best;
    bestMatch.block = best;

    return bestMatch;
}
//
// Created by Lukas Kellenberger on 01.03.17.
//

#include <stdlib.h>
#include <printf.h>
#include "linked_list.h"


LinkedList* linked_list_create(size_t size) {
    LinkedList* list = malloc(sizeof(LinkedList));
    list->first = NULL;
    list->last = NULL;
    return list;
}

void linked_list_add_block(LinkedList* list, Block* block, size_t block_size) {
    block->header.size = block_size;
    block->header.tag = tag_free;

    assert(block_size >= 1);

    if(list->first == NULL) {
        list->first = block;
    } else {
        list->last->next = block;
    }
    block->next = LIST_END;
    list->last = block;
}
void linked_list_remove_block(LinkedList* list, Block* block, size_t block_size, Block* previous) {
    assert(block != NULL);

    Block* next = block->next;

    if (list->first == block) {
        // If the block is the first element of the list
        list->first = next == LIST_END ? LIST_END : next;
    } else {
        previous->next = next;
    }

    if(list->last == block) {
        list->last = next == LIST_END ? previous : next;
    }

    block->header.size = block_size;
    block->header.tag = tag_allocated;
}

void linked_list_print(LinkedList* list) {
    Block* current = list->first;
    printf("list: ");
    while(current != NULL) {
        size_t size = current->header.size + 1;
        printf("[%p (%zu)] -> ", current, size);
        current = current->next;
    }
    printf("\n");
}

void linked_list_split_block(LinkedList* list, Block* block, size_t size) {
    assert(size % sizeof(word_t) == 0);
    size_t size_with_header = size / sizeof(word_t) + 1;
    // minimal block size is 2
    assert(size_with_header > 1);

    size_t block_size_with_header = block->header.size + 1;


    assert(size_with_header + 2 <= block_size_with_header);
    //remaining block size must be at least 2
    size_t remaining_size_with_header = block_size_with_header - size_with_header;
    assert(remaining_size_with_header > 1);
    Block* next = block->next;

    Block* remaining_free_block = block_add_offset(block, size_with_header);

    block->header.size = size_with_header - 1;
    block->header.tag = tag_free;

    block->next = remaining_free_block;

    remaining_free_block->header.size = remaining_size_with_header - 1;
    remaining_free_block->header.tag = tag_free;

    remaining_free_block->next = next;
}

/*
 * size without header in bytes
 */
BestMatch linked_list_find_block(LinkedList* list, size_t size) {
    assert(size % sizeof(word_t) == 0);
    size_t nb_words_with_header = size / sizeof(word_t) + 1;

    Block* previous_best = NULL;
    Block* best = NULL;
    size_t best_size = SIZE_MAX;
    Block* previous = NULL;
    Block* current = list->first;
    while(current != NULL) {
        size_t current_size_with_header = current->header.size + 1;
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
        current = current->next;
    }


    BestMatch bestMatch;
    bestMatch.previous = previous_best;
    bestMatch.block = best;

    return bestMatch;
}

BestMatch linked_list_find_first_block(LinkedList* list, size_t size) {
    assert(size % sizeof(word_t) == 0);
    size_t nb_words_with_header = size / sizeof(word_t) + 1;

    Block* previous_best = NULL;
    Block* best = NULL;
    size_t best_size = SIZE_MAX;
    Block* previous = NULL;
    Block* current = list->first;
    while(current != NULL) {
        size_t current_size_with_header = current->header.size + 1;
        if(current_size_with_header >= nb_words_with_header) {
            best = current;
            previous_best = previous;
            break;
        }
        previous = current;
        current = current->next;
    }


    BestMatch bestMatch;
    bestMatch.previous = previous_best;
    bestMatch.block = best;

    return bestMatch;
}
//
// Created by Lukas Kellenberger on 01.03.17.
//

#include <stdlib.h>
#include <printf.h>
#include "linked_list.h"

LinkedList* linked_list_alloc() {
    LinkedList* list = malloc(sizeof(LinkedList));
    list->first = NULL;
    list->last = NULL;
    return list;
}


void linked_list_add_block(LinkedList* list, Block* block, size_t block_size) {
    // Set the header fields
    block->header.size = block_size;
    block->header.tag = tag_free;

    assert(block_size >= 1);

    // If the list is empty, set the block as first block
    if(list->first == NULL) {
        list->first = block;
    } else {
        // Otherwise the block is the next of the last block
        list->last->next = block;
    }
    // Because the block is added at the end, there is no next and last is the added block
    block->next = LIST_END;
    list->last = block;
}

void linked_list_remove_block(LinkedList* list, Block* block, size_t block_size, Block* previous) {
    assert(block != NULL);
    assert(list->first == block || previous != NULL);
    assert(previous == NULL || previous->next == block);

    Block* next = block->next;

    // If the block is the first element of the list
    if (previous == NULL) {
        list->first = next;
    } else {
        previous->next = next;
    }

    // If the block to remove was the last block of the linked_list, update last.
    if(list->last == block) {
        assert(next == LIST_END);
        list->last = previous;
    }

    block->header.size = block_size;
    block->header.tag = tag_allocated;
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
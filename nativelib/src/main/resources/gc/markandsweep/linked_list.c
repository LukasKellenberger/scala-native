//
// Created by Lukas Kellenberger on 01.03.17.
//

#include <stdlib.h>
#include <printf.h>
#include "linked_list.h"

// Creates an empty linked list
LinkedList* linked_list_create() {
    LinkedList* list = malloc(sizeof(LinkedList));
    list->first = NULL;
    list->last = NULL;
    return list;
}

// Adds a block at the end of the given linked list
void linked_list_add_block(LinkedList* list, Block* block, size_t block_size) {
    //Set the size of the block and mark it as free
    block->header.size = block_size;
    block->header.tag = tag_free;

    assert(block_size >= 2);

    // If the list is empty, set block as first element otherwise set it as next of the last element
    if(list->first == NULL) {
        list->first = block;
    } else {
        list->last->next = block;
    }
    // Set list_end to next and set it as last block
    block->next = LIST_END;
    list->last = block;
}

// Removes a block from the the linked list. If previous is NULL, it assumes that the block is the first element
void linked_list_remove_block(LinkedList* list, Block* block, size_t block_size, Block* previous) {
    assert(block != NULL);
    assert(block_size > 1);
    assert(list->first == block || previous != NULL);

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

    assert(block_size > 1);
    block->header.size = block_size;
    block->header.tag = tag_allocated;
}

void linked_list_print(LinkedList* list) {
    Block* current = list->first;
    printf("list: ");
    while(current != NULL) {
        size_t size = current->header.size;
        assert(size > 1);
        printf("[%p (%zu)] -> ", current, size);
        current = current->next;
    }
    printf("\n");
}

void linked_list_check(LinkedList* list, int expectedSize, Bitmap* bitmap) {
    Block* current = list->first;
    while(current != NULL) {
        size_t size = current->header.size;
        assert(size > 1);
        //assert(expectedSize == -1 || expectedSize == size);
        for(word_t* c = (word_t*)current + 1; c < (word_t*)current + size; c +=1) {
            assert(!bitmap_get_bit(bitmap, c));
        }
        current = current->next;
    }
}

void linked_list_split_block(LinkedList* list, Block* block, size_t size_first) {
    // minimal block size is 2
    assert(size_first > 1);

    size_t total_size = block->header.size;


    assert(total_size - size_first > 1);
    //remaining block size must be at least 2
    size_t size_second = total_size - size_first;
    assert(size_second > 1);
    Block* next = block->next;

    Block* second_block = block_add_offset(block, size_first);

    block->header.size = size_first;
    block->header.tag = tag_free;

    block->next = second_block;

    second_block->header.size = size_second;
    second_block->header.tag = tag_free;

    second_block->next = next;
}

/*
 * size without header in bytes
 */
BestMatch linked_list_find_block(LinkedList* list, size_t size) {

    Block* previous_best = NULL;
    Block* best = NULL;
    size_t best_size = SIZE_MAX;
    Block* previous = NULL;
    Block* current = list->first;
    while(current != NULL) {
        size_t current_size = current->header.size;
        if(current_size == size) {
            best = current;
            previous_best = previous;
            break;
        } else if(current_size > size && current_size < best_size) {
            best = current;
            best_size = current_size;
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
    Block* previous_best = NULL;
    Block* best = NULL;
    size_t best_size = SIZE_MAX;
    Block* previous = NULL;
    Block* current = list->first;
    while(current != NULL) {
        size_t current_size = current->header.size;
        if(current_size >= size) {
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
//
// Created by Lukas Kellenberger on 01.03.17.
//

#include <stddef.h>
#include "block.h"
#include "types.h"

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct {
    word_t* start;
    word_t* first;
    word_t* last;
} LinkedList;

#define LIST_END 0LLU


typedef struct {
    word_t* block;
    word_t* previous;
} BestMatch;

/**
 *
 * Creates a new LinkedList with one block of size `size` at address start
 *
 * @param size of the first block
 * @return pointer to the Linkedlist
 */
LinkedList* linked_list_create(size_t size, word_t* start);

/**
 * Adds a block to the end of the list, does not merge blocks.
 */
void linked_list_add_block(LinkedList* list, word_t* block, size_t block_size);

/**
 * Removes the block `block` from the linked list
 */
void linked_list_remove_block(LinkedList* list, word_t* block, size_t size, word_t* previous);

/**
 * Returns the next block after `block` in the linked list
 */
word_t* linked_list_next(LinkedList* list, word_t* block);

/**
 * Prints the linked list
 */
void linked_list_print(LinkedList* list);

void linked_list_split_block(LinkedList* list, word_t* block, size_t size);


BestMatch linked_list_find_block(LinkedList* list, size_t size);
BestMatch linked_list_find_first_block(LinkedList* list, size_t size);



#endif //LINKED_LIST_H

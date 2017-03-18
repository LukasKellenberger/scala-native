//
// Created by Lukas Kellenberger on 01.03.17.
//

#include <stddef.h>
#include "block.h"
#include "types.h"
#include "bitmap.h"


#ifndef LINKED_LIST_H
#define LINKED_LIST_H


typedef struct {
    Block* first;
    Block* last;
} LinkedList;

#define LIST_END NULL


typedef struct {
    Block* block;
    Block* previous;
} BestMatch;

/**
 *
 * Creates a new LinkedList with one block of size `size` at address start
 *
 * @param size of the first block
 * @return pointer to the Linkedlist
 */
LinkedList* linked_list_create(size_t size);

/**
 * Adds a block to the end of the list, does not merge blocks.
 */
void linked_list_add_block(LinkedList* list, Block* block, size_t block_size);

/**
 * Removes the block `block` from the linked list
 */
void linked_list_remove_block(LinkedList* list, Block* block, size_t size, Block* previous);

/**
 * Prints the linked list
 */
void linked_list_print(LinkedList* list);

void linked_list_check(LinkedList* list, int expectedSize, Bitmap* bitmap);

void linked_list_split_block(LinkedList* list, Block* block, size_t size);


BestMatch linked_list_find_block(LinkedList* list, size_t size);
BestMatch linked_list_find_first_block(LinkedList* list, size_t size);
BestMatch linked_list_find_good_block(LinkedList* list, size_t size);



#endif //LINKED_LIST_H

//
// Created by Lukas Kellenberger on 01.03.17.
//

#include <stddef.h>
#include "block.h"
#include "types.h"

#ifndef UNTITLED_LINKED_LIST_H
#define UNTITLED_LINKED_LIST_H

typedef struct {
    size_t size;
    word_t* start;
    word_t* first;
} LinkedList;

#define LIST_END 0LLU


/**
 *
 * Creates a new LinkedList by allocating a block size `size`
 *
 * @param size number of bytes to alloc (must by a mutiple of sizeof(word_t))
 * @return pointer to the Linkedlist
 */
LinkedList* linked_list_alloc(size_t size);
word_t* linked_list_add_block(LinkedList* list, word_t* block, word_t* previous);
void linked_list_remove_block(LinkedList* list, word_t* block, word_t* previous);
word_t* linked_list_next(LinkedList* list, word_t* block);
void linked_list_split_block(LinkedList* list, word_t* block, size_t size);
void linked_list_print(LinkedList* list);

typedef struct {
    word_t* block;
    word_t* previous;
} BestMatch;

BestMatch linked_list_find_block(LinkedList* list, size_t size);

word_t addr_p_to_v(LinkedList* list, word_t* p_addr);

#endif //UNTITLED_LINKED_LIST_H

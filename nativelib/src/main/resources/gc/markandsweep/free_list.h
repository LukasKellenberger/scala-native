//
// Created by Lukas Kellenberger on 01.03.17.
//

#include "bitmap.h"
#include "types.h"
#include "linked_list.h"

#ifndef MARKANDSWEEP_FREE_LIST_H
#define MARKANDSWEEP_FREE_LIST_H

#define SMALLEST_BLOCK_SIZE 2
#define MAX_CONST_SIZE_LIST 16
#define POWER_MAX_CONST_SIZE 4
#define LINKED_LIST_NUMBER 32


typedef struct {
    LinkedList* list[LINKED_LIST_NUMBER];
    Bitmap* bitmap;
    word_t* start;
    size_t size;
} FreeList;

/**
 *
 * Allocates a block of size `size` bytes and creates the free_list
 */
FreeList* free_list_create(size_t size);

/**
 * Adds a block to one of the linked_lists, depending on the size of the block.
 *
 *
 */
void free_list_add_block(FreeList* list, word_t* block, size_t block_size);

/**
 * Returns a block of size `size` or `size + 8`.
 * If there is no block of size `size` or `size + 8` split a larger block.
 * If there is no larger block, return NULL
 */
word_t* free_list_get_block(FreeList* list, size_t size);


void free_list_print(FreeList* list);

void free_list_clear_lists(FreeList* list);

/*MemoryStats free_list_get_stats(FreeList* list);

void free_list_print_stats(FreeList* list);

void print_memory(FreeList* free_list);*/



#endif //MARKANDSWEEP_FREE_LIST_H

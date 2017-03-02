//
// Created by Lukas Kellenberger on 01.03.17.
//

#include "bitmap.h"
#include "types.h"
#include "linked_list.h"

#ifndef MARKANDSWEEP_FREE_LIST_H
#define MARKANDSWEEP_FREE_LIST_H



typedef struct {
    LinkedList* list;
    Bitmap* bitmap;
} FreeList;

typedef struct {
    size_t available_words;
    size_t largest_word;
    long nb_words;
} MemoryStats;

FreeList* free_list_create(size_t size);

word_t* free_list_add_block(FreeList* list, word_t* block, word_t* previous_block);

word_t* free_list_get_block(FreeList* list, size_t size);

word_t* free_list_next(FreeList* free_list, word_t* node);

void free_list_print(FreeList* list);

MemoryStats free_list_get_stats(FreeList* list);

void free_list_print_stats(FreeList* list);

void print_memory(FreeList* free_list);



#endif //MARKANDSWEEP_FREE_LIST_H

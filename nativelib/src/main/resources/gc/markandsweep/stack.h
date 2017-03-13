

#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stddef.h>
#include "types.h"

typedef word_t* Stack_Type;

typedef struct {
    Stack_Type* bottom;
    int nb_words;
    int current;
} Stack;

Stack* stack_alloc(size_t size);

void stack_push(Stack* stack, Stack_Type word);

Stack_Type stack_pop(Stack* stack);

int stack_is_empty(Stack* stack);

#endif //STACK_H
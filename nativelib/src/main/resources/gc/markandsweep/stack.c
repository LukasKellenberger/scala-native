#include "stack.h"

Stack* stack_alloc(size_t size) {
    assert(size % sizeof(Stack_Type) == 0);
    Stack* stack = malloc(sizeof(Stack));
    stack->current = 0;
    stack->bottom = malloc(size);
    stack->nb_words = size / sizeof(Stack_Type);
    return stack;
}

void stack_push(Stack* stack, Stack_Type word) {
    assert(stack->current < stack->nb_words);
    stack->bottom[stack->current++] = word;
}

Stack_Type stack_pop(Stack* stack) {
    assert(stack->current > 0);
    return stack->bottom[--stack->current];
}

int stack_is_empty(Stack* stack) {
    return stack->current == 0;
}
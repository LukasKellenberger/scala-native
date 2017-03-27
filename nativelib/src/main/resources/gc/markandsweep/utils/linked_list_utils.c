#include "linked_list_utils.h"

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

void linked_list_check(LinkedList* list, int expectedSize, Bitmap* bitmap) {
    Block* current = list->first;
    while(current != NULL) {
        size_t size = current->header.size + 1;
        assert(expectedSize == -1 || expectedSize == size);
        for(word_t* c= (word_t*)current + 1; c < (word_t*)current + size; c +=1) {
            assert(!bitmap_get_bit(bitmap, c));
        }
        current = current->next;
    }
}
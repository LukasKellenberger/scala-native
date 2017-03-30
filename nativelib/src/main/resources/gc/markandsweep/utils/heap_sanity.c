#include "heap_sanity.h"


int check_is_header_in_linked_list(LinkedList* list, word_t* block) {
    Block* current = list->first;
    int found = 0;
    while(current != NULL) {
        word_t* current_as_word_t = (word_t*) current;
        size_t size = header_unpack_block_size(current_as_word_t) - 1;
        if(current_as_word_t == block) found = 1;
        assert(!(block > current_as_word_t && block < current_as_word_t + size));
        current = current->next;
    }
    return found;
}

void check_is_header_in_free_list(FreeList* list, word_t* block) {
    int found = 0;
    for(int i=0; i < LINKED_LIST_NUMBER; i++) {
        if(list->list[i]->first != NULL) {
            int newFound = check_is_header_in_linked_list(list->list[i], block);
            assert(!newFound || !found);
            found |= newFound;
        }
    }

    assert(found);
}

void check_not_in_linked_list(LinkedList* list, word_t* block) {
    Block* current = list->first;
    while(current != NULL) {
        word_t* current_as_word_t = (word_t*) current;
        size_t size = header_unpack_block_size(current_as_word_t) - 1;
        assert(!(block >= current_as_word_t && block < current_as_word_t + size));
        current = current->next;
    }
}

void check_not_in_free_list(FreeList* list, word_t* block) {
    for(int i=0; i < LINKED_LIST_NUMBER; i++) {
        if(list->list[i]->first != NULL) {
            check_is_header_in_linked_list(list->list[i], block);
        }
    }
}

void check_header(Heap* heap, word_t* block) {
    FreeList* freeList = heap->free_list;
    size_t size = header_unpack_block_size(block);
    assert(size > 1);

    tag_t tag = header_unpack_tag(block);
    assert(tag == tag_allocated || tag == tag_free);

    assert(bitmap_get_bit(freeList->bitmap, block));

    for (int i = 1; i < size; i++) {
        assert(!bitmap_get_bit(freeList->bitmap, block + i));
    }

    assert(block + size == heap->heap_end || bitmap_get_bit(freeList->bitmap, block + size));

    if(tag == tag_allocated) {
        check_not_in_free_list(freeList, block);
    } else {
        check_is_header_in_free_list(freeList, block);
    }
}

void heap_sanity_full_check(Heap* heap) {
    word_t* current = heap->heap_start;
    while(current != NULL) {
        check_header(heap, current);
        current = heap_next_block(heap, current);
    }
}

void memory_check(FreeList* free_list, int print) {
    Bitmap* bitmap = free_list->bitmap;
    word_t* current = bitmap->offset;
    word_t* previous = NULL;
    size_t previous_size = 0;

    for(int i=0; i < bitmap->size / sizeof(word_t); i++) {
        if(bitmap_get_bit(bitmap, current)) {
            size_t size = header_unpack_block_size(current) - 1;
            if(header_unpack_tag(current) == tag_allocated && print) {
                printf("|A %p %zu", current, size);
            } else if(header_unpack_tag(current) == tag_free && print) {
                printf("|F %p %zu", current, size);
            }
            fflush(stdout);
            if(previous != NULL) {
                assert(previous + previous_size + 1 == current);
            } else {
                assert(current == bitmap->offset);
            }
            previous_size = size;
            previous = current;
        }
        current += 1;
    }
}
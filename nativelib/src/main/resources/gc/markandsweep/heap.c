#include "heap.h"


Heap* heap_alloc(size_t size) {
    Heap* heap = malloc(sizeof(Heap));
    size_t nb_words = size / sizeof(word_t);

    word_t* heap_start = calloc(nb_words, sizeof(word_t));

    heap->nb_words = nb_words;
    heap->heap_start = heap_start;
    heap->heap_end = heap_start + nb_words;

    Bitmap* bitmap = bitmap_alloc(size, heap_start);
    Bitmap* bitmap_copy = bitmap_alloc(size, heap_start);
    heap->bitmap = bitmap;
    heap->bitmap_copy = bitmap_copy;

    heap->free_list = free_list_create(nb_words, heap_start, bitmap);

    return heap;
}

int heap_in_heap(Heap* heap, word_t* block) {
    return block >= heap->heap_start && block < heap->heap_end;
}

int heap_cannot_be_const(Heap* heap, word_t* block) {
    return block >= heap->heap_start;
}

word_t* heap_next_block(Heap* heap, word_t* block) {
    assert(heap_in_heap(heap, block));
    size_t block_size = header_unpack_size(block);
    word_t* next = block + block_size + 1;

    return next == heap->heap_end ? NULL : next;
}

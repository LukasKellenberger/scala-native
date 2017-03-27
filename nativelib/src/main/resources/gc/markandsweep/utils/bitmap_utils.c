#include "bitmap_utils.h"


void bitmap_print(Bitmap* bitmap) {

    size_t nb_words = bitmap->size / sizeof(word_t);
    word_t* current = bitmap->offset;
    printf("bitmap: ");
    for(unsigned long i=0; i < nb_words; i++) {
        if(bitmap_get_bit(bitmap, current)) {
            printf("%lu ", i);
        }
        current = current + 1;
    }
    printf("\n");
}

void bitmap_print_with_rtti(Bitmap* bitmap) {

    size_t nb_words = bitmap->size / sizeof(word_t);
    word_t* current = bitmap->offset;
    printf("bitmap: ");
    for(unsigned long i=0; i < nb_words; i++) {
        if(bitmap_get_bit(bitmap, current)) {
            Rtti* rtti = (Rtti*)(*(current + 1));
            printf("%lu:%d:%zu:%d ", i, rtti == NULL ? -1 : rtti->id, header_unpack_size(current), header_unpack_tag(current));
        }
        current = current + 1;
    }
    printf("\n");
}
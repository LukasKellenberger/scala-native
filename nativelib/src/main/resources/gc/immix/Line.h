#ifndef IMMIX_LINE_H
#define IMMIX_LINE_H


#include "headers/ObjectHeader.h"
#include "headers/LineHeader.h"

ObjectHeader* line_header_getFirstObject(LineHeader*);
void line_header_update(word_t* objectStart, word_t* objectEnd);

#endif //IMMIX_LINE_H

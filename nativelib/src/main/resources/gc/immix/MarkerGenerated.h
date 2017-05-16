#ifndef IMMIX_MARKER_GENERATED_H
#define IMMIX_MARKER_GENERATED_H

#include "headers/ObjectHeader.h"
#include "datastructures/Stack.h"
#include "Heap.h"
#include "Marker.h"

INLINE void markerGenerated_mark(Heap* heap, Stack* stack, ObjectHeader* object, ubyte_t ptrByte, int offset) {
    switch (ptrByte) {

        case 0:
            break;
        case 1:
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 2:
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 3:
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 4:
            marker_markField(heap, stack, object, offset + 4);
            break;
        case 5:
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 6:
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 7:
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 8:
            marker_markField(heap, stack, object, offset + 3);
            break;
        case 9:
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 10:
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 11:
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 12:
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            break;
        case 13:
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 14:
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 15:
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 16:
            marker_markField(heap, stack, object, offset + 2);
            break;
        case 17:
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 18:
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 19:
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 20:
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 4);
            break;
        case 21:
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 22:
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 23:
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 24:
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            break;
        case 25:
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 26:
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 27:
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 28:
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            break;
        case 29:
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 30:
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 31:
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 32:
            marker_markField(heap, stack, object, offset + 1);
            break;
        case 33:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 34:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 35:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 36:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 4);
            break;
        case 37:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 38:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 39:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 40:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 3);
            break;
        case 41:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 42:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 43:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 44:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            break;
        case 45:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 46:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 47:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 48:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            break;
        case 49:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 50:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 51:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 52:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 4);
            break;
        case 53:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 54:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 55:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 56:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            break;
        case 57:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 58:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 59:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 60:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            break;
        case 61:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 62:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 63:
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 64:
            marker_markField(heap, stack, object, offset + 0);
            break;
        case 65:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 66:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 67:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 68:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 4);
            break;
        case 69:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 70:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 71:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 72:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 3);
            break;
        case 73:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 74:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 75:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 76:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            break;
        case 77:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 78:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 79:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 80:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 2);
            break;
        case 81:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 82:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 83:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 84:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 4);
            break;
        case 85:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 86:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 87:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 88:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            break;
        case 89:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 90:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 91:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 92:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            break;
        case 93:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 94:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 95:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 96:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            break;
        case 97:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 98:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 99:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 100:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 4);
            break;
        case 101:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 102:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 103:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 104:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 3);
            break;
        case 105:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 106:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 107:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 108:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            break;
        case 109:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 110:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 111:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 112:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            break;
        case 113:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 114:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 115:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 116:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 4);
            break;
        case 117:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 118:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 119:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 120:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            break;
        case 121:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 122:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 123:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 124:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            break;
        case 125:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 6);
            break;
        case 126:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            break;
        case 127:
            marker_markField(heap, stack, object, offset + 0);
            marker_markField(heap, stack, object, offset + 1);
            marker_markField(heap, stack, object, offset + 2);
            marker_markField(heap, stack, object, offset + 3);
            marker_markField(heap, stack, object, offset + 4);
            marker_markField(heap, stack, object, offset + 5);
            marker_markField(heap, stack, object, offset + 6);
            break;
        default:
            break;
    }
}


#endif //IMMIX_MARKER_GENERATED_H
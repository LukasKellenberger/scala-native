//
// Created by Lukas Kellenberger on 11.04.17.
//

#include "Immix_space.h"
#include "utility/line.h"


int immix_space_get_next_unavailable_line(ImmixSpace* immixSpace, void* baseLineAvailAddress, int line) {
    return line_get_next_unavailable(baseLineAvailAddress, line, immixSpace->lineUnavailState);
}

int immix_space_get_next_available_line(ImmixSpace* immixSpace, void* baseLineAvailAddress, int line) {
    return line_get_next_available(baseLineAvailAddress, line, immixSpace->lineUnavailState);

}
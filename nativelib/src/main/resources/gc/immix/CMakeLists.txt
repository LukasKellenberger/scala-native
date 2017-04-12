cmake_minimum_required(VERSION 3.6)
project(immix)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        immix_allocator.c
        immix_allocator.h utility/address_utils.c utility/address_utils.h utility/block.c utility/block.h utility/types.h utility/immix_constants.h utility/constants.h Immix_space.c Immix_space.h utility/line.c utility/line.h utility/chunk.c utility/chunk.h)

add_executable(immix ${SOURCE_FILES})
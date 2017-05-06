cmake_minimum_required(VERSION 3.6)
project(immix)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        datastructures/Bitmap.c
        datastructures/Bitmap.h
        datastructures/BlocList.c
        datastructures/BlocList.h
        headers/BlockHeader.h
        GCTypes.h
        headers/LineHeader.h
        headers/ObjectHeader.h
        utils/MathUtils.h
        Allocator.c
        Allocator.h
        Block.c
        Block.h
        Constants.h
        Heap.c
        Heap.h
        ImmixGC.c
        LargeAllocator.c
        LargeAllocator.h
        Line.c
        Line.h
        Marker.c
        Marker.h
        Object.c
        Object.h
        datastructures/Stack.c
        datastructures/Stack.h Log.h)

add_executable(immix ${SOURCE_FILES})
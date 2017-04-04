#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <signal.h>
#include <stdint.h>

// Dummy GC that maps 1tb of memory and allocates but never frees.

void* current = (void*)0x300000000000;

void* start = (void*)0x300000000000;

size_t total = 0;

// Map 1TB
#define CHUNK (512*1024*1024L)
// Allow read and write
#define DUMMY_GC_PROT (PROT_READ | PROT_WRITE)
// Map private anonymous memory, and prevent from reserving swap
#define DUMMY_GC_FLAGS (MAP_NORESERVE | MAP_PRIVATE | MAP_ANONYMOUS)
// Map anonymous memory (not a file)
#define DUMMY_GC_FD -1
#define DUMMY_GC_FD_OFFSET 0


void handle_sigsegv()
{
    printf("signal %p %p\n", current, start);
    fflush(stdout);
    
    size_t diff = (uintptr_t)current - (uintptr_t)start;
    void* last = (void*)((uintptr_t)current - (diff % CHUNK));
    printf("last %p\n", last);
    fflush(stdout);
    void* m = mmap(last, CHUNK, DUMMY_GC_PROT, DUMMY_GC_FLAGS | MAP_FIXED, DUMMY_GC_FD, DUMMY_GC_FD_OFFSET);
    printf("mmap returned\n");
    fflush(stdout);
    total += CHUNK;
    printf("returned %p\n", m);
    if(total > 100*CHUNK) {
        exit(1);
    }
}


void scalanative_init() {
    signal(SIGSEGV, handle_sigsegv);
    current = mmap(start, CHUNK, DUMMY_GC_PROT, DUMMY_GC_FLAGS | MAP_FIXED, DUMMY_GC_FD, DUMMY_GC_FD_OFFSET);
    start = current;
}

// Allocates without bound checks, fails once it runs out of memory
void* scalanative_alloc_raw(size_t size) {
    size = size + (8 - size % 8);
    if (current != 0) {
        void* alloc = current;
        current += size;
        return alloc;
    } else {
        scalanative_init();
        return scalanative_alloc_raw(size);
    }
}

void* scalanative_alloc_raw_atomic(size_t size) {
    return scalanative_alloc_raw(size);
}

void* scalanative_alloc(void* info, size_t size) {
    void** alloc = (void**) scalanative_alloc_raw(size);
    *alloc = info;
    return (void*) alloc;
}

void scalanative_collect() {}

#ifndef MEMORY_HEAP_H
#define MEMORY_HEAP_H
#include "typedefs.h"

typedef struct MemorySegmentHeader MemorySegmentHeader_t;

typedef struct MemorySegmentHeader {
    uint64_t                Size;
    MemorySegmentHeader_t*  NextSegment;
    MemorySegmentHeader_t*  NextFreeSegment;
    MemorySegmentHeader_t*  PrevSegment;
    MemorySegmentHeader_t*  PrevFreeSegment;
    BOOL                    Free;
} MemorySegmentHeader_t;

typedef struct MemoryHeap {
    MemorySegmentHeader_t*  FirstSegment;
    MemorySegmentHeader_t*  FirstFreeSegment;
    uint64_t                Size;
} MemoryHeap_t;

// Sets up a free memory heap at the
// specified address with the provided size.
MemoryHeap_t kCreateHeap(uint64_t base, uint64_t size);

// Allocates specified number of bytes
// on the kernel heap with the given alignment.
void* kMemoryAllocWithAlignment(MemoryHeap_t* heap, uint64_t size, uint64_t alignment);

// Allocates specified number of bytes on the 
// kernel heap with the default alignment of 8 bytes.
void* kMemoryAlloc(MemoryHeap_t* heap, uint64_t size);

// Allocates memory on the heap with the given
// alignment, but also initializes all bytes to be 0.
void* kClearMemoryAllocWithAlignment(MemoryHeap_t* heap, uint64_t size, uint64_t alignment);

// Allocates memory on the heap with default alignment
// of 8 bytes, but also initializes all bytes to be 0.
void* kClearMemoryAlloc(MemoryHeap_t* heap, uint64_t size);

// Frees the allocated memory segment and returns
// it to the heap to use for later allocations.
void kFreeMemory(MemoryHeap_t* heap, void* ptr);

#endif

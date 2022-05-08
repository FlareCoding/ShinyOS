#ifndef KERNEL_MEMORY_H
#define KERNEL_MEMORY_H
#include "typedefs.h"
#include "MemoryHeap.h"

// Copies the specified number of bytes from the src
// address to the destination address.
void kCopyMemory(void* dest, void* src, size_t size);

// Sets the specified number of bytes at the
// destination address to the specified value.
void kSetMemory(void* dest, uint8_t value, size_t size);

#endif

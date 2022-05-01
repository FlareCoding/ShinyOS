#ifndef KERNEL_MEMORY_H
#define KERNEL_MEMORY_H
#include "typedefs.h"

void kCopyMemory(void* dest, void* src, size_t size);

void kSetMemory(void* dest, uint8_t value, size_t size);

#endif

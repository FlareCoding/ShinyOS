#include "KernelMemory.h"

void kCopyMemory(void* dest, void* src, size_t size)
{
    uchar_t* cdest = (uchar_t*)dest;
    uchar_t* csrc  = (uchar_t*)src;

    size_t i;
    for (i = 0; i < size; ++i)
        cdest[i] = csrc[i];
}

void kSetMemory(void* dest, uint8_t value, size_t size)
{
    uchar_t* cdest = (uchar_t*)dest;

    size_t i;
    for (i = 0; i < size; ++i)
        cdest[i] = value;
}

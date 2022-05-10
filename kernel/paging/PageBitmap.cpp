#include "PageBitmap.h"

bool PageBitmap::operator[](uint64_t idx)
{
    if (idx > Size * 8) return false;

    uint64_t byte_idx   = idx / 8;
    uint8_t  bit_idx    = idx % 8;
    uint8_t  mask       = 0b10000000 >> bit_idx;

    return Buffer[byte_idx] & mask;
}

bool PageBitmap::Set(uint64_t idx, bool val)
{
    if (idx > Size * 8) return false;

    uint64_t byte_idx   = idx / 8;
    uint8_t  bit_idx    = idx % 8;
    uint8_t  mask       = 0b10000000 >> bit_idx;

    Buffer[byte_idx] &= ~mask;

    if (val)
        Buffer[byte_idx] |= mask;

    return true;
}

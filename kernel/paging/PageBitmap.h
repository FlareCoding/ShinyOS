#pragma once
#include <kernel/typedefs.h>

class PageBitmap
{
public:
    PageBitmap() = default;
    ~PageBitmap() = default;

    uint64_t Size;
    uint8_t* Buffer;

    bool operator[](uint64_t idx);
    bool Set(uint64_t idx, bool val);
};

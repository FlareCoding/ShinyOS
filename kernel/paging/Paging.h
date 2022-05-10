#pragma once
#include <kernel/typedefs.h>

#define PAGE_SIZE           4096
#define PAGE_TABLE_ENTRIES  512

enum class PT_Flags
{
    Present = 0,
    Writable = 1,
    Usermode = 2,
    WriteThrough = 3,
    CacheDisabled = 4,
    Accessed = 5,
    LargerPages = 7,
    CustomBitR = 9,
    CustomBitM = 10,
    CustomBitL = 11,
    NoExecuteBit = 63
};

struct PageTableEntry
{
    uint64_t Flags;

    void SetFlag(PT_Flags fl, bool val);
    bool GetFlag(PT_Flags fl);

    void SetAddress(uint64_t address);
    uint64_t GetAddress();
};

struct PageTable
{
    PageTableEntry entries[PAGE_TABLE_ENTRIES];
} __attribute__((aligned(PAGE_SIZE)));

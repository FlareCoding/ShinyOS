#include "Paging.h"

void PageTableEntry::SetFlag(PT_Flags fl, bool val)
{
    uint64_t mask = 1ULL << static_cast<uint64_t>(fl);

    // Clear the bit
    Flags &= ~mask;

    // Set the bit
    if (val)
        Flags |= mask;
}

bool PageTableEntry::GetFlag(PT_Flags fl)
{
    uint64_t mask = 1ULL << static_cast<uint64_t>(fl);

    return Flags & mask;
}

void PageTableEntry::SetAddress(uint64_t address)
{
    address &= 0x000000ffffffffff;

    // Clear the 12 address bits
    Flags &= 0xfff0000000000fff;

    // Set the address bits
    Flags |= (address << 12);
}

uint64_t PageTableEntry::GetAddress()
{
    return (Flags & 0x000ffffffffff000) >> 12;
}

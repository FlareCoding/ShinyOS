#include "MemoryMap.h"

uint8_t _MemoryMapAvailableMemoryRegions = 0;
uint8_t _MemoryMapUsableMemoryRegions = 0;
uint8_t _MemoryMapReservedMemoryRegions = 0;

MemoryMapEntry_t* g_ReservedMemoryRegions[16];
MemoryMapEntry_t* g_UsableMemoryRegions[16];

void kCalculateMemoryMapRegions()
{
    uint64_t usable_region_idx = 0;
    uint64_t reserved_region_idx = 0;

    for (uint8_t i = 0; i < 32; i++)
    {
        MemoryMapEntry_t* Entry = (MemoryMapEntry_t*)MEMORY_MAP_ADDRESS;
        Entry += i;

        if (Entry->RegionBase == 0 &&
            Entry->RegionSize == 0 &&
            Entry->RegionType == 0)
            break;

        ++_MemoryMapAvailableMemoryRegions;

        // If the region type is 1, that means
        // the kernel can use it for its heap.
        if (Entry->RegionType == 1)
        {
            g_UsableMemoryRegions[usable_region_idx] = Entry;
            
            ++usable_region_idx;
            ++_MemoryMapUsableMemoryRegions;
        }
        else
        {
            g_ReservedMemoryRegions[reserved_region_idx] = Entry;
            
            ++reserved_region_idx;
            ++_MemoryMapReservedMemoryRegions;
        }
    }
}

uint64_t kGetTotalMemorySize()
{
    static uint64_t s_TotalMemorySize = 0;

    if (s_TotalMemorySize > 0)
        return s_TotalMemorySize;

    for (uint8_t i = 0; i < 32; i++)
    {
        MemoryMapEntry_t* Entry = (MemoryMapEntry_t*)MEMORY_MAP_ADDRESS;
        Entry += i;

        s_TotalMemorySize += Entry->RegionSize;
    }

    return s_TotalMemorySize;
}

MemoryMapEntry_t** kGetUsableMemoryRegions()
{
    return g_UsableMemoryRegions;
}

MemoryMapEntry_t** kGetReservedMemoryRegions()
{
    return g_ReservedMemoryRegions;
}

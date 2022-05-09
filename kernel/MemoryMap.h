#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H
#include "typedefs.h"

#define MEMORY_MAP_ADDRESS 0x5000

struct MemoryMapEntry_t {
    uint64_t RegionBase;
    uint64_t RegionSize;
    uint32_t RegionType;
    uint32_t ExtendedAttribs;
};

// Calculates the number of regions in the memory map
void kCalculateMemoryMapRegions();

// Returns the list of memory regions
// available to be used by the kernel.
MemoryMapEntry_t** kGetUsableMemoryRegions();

extern uint8_t _MemoryMapAvailableMemoryRegions;
extern uint8_t _MemoryMapUsableMemoryRegions;

#endif

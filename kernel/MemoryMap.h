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

// Returns the list of the reserved memory regions
MemoryMapEntry_t** kGetReservedMemoryRegions();

// Returns the total memory size recorded in the memory map
uint64_t kGetTotalMemorySize();

extern uint8_t _MemoryMapAvailableMemoryRegions;
extern uint8_t _MemoryMapUsableMemoryRegions;
extern uint8_t _MemoryMapReservedMemoryRegions;

#endif

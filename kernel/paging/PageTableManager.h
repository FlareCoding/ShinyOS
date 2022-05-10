#pragma once
#include "Paging.h"
#include "PageFrameAllocator.h"

enum class MemoryPageAttribs
{
    READONLY,
    READ_WRITE
};

enum class MemoryPagePrivilegeLevel
{
    KERNEL,
    USERMODE
};

class PageTableManager
{
public:
    static PageTable* GetGlobalPML4Table();

    PageTableManager(PageTable* PageMapLevel4Address);
    ~PageTableManager() = default;

    void MapMemory(void* pVirtual, void* pPhysical, MemoryPageAttribs attribs, MemoryPagePrivilegeLevel privilege_lvl);

public:
    PageTable* PageMapLevel4Table;
};

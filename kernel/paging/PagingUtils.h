#pragma once
#include "Paging.h"

class PageTableIndexer
{
public:
    PageTableIndexer(uint64_t virtual_address);

    uint64_t PageDirectoryPointerIndex;
    uint64_t PageDirectoryIndex;
    uint64_t PageTableIndex;
    uint64_t PageIndex;
};

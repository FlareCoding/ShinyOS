#include "PagingUtils.h"

PageTableIndexer::PageTableIndexer(uint64_t virtual_address)
{
    uint64_t mask = 0x1ff;

    virtual_address >>= 12;
    PageIndex = virtual_address & mask;

    virtual_address >>= 9;
    PageTableIndex = virtual_address & mask;

    virtual_address >>= 9;
    PageDirectoryIndex = virtual_address & mask;

    virtual_address >>= 9;
    PageDirectoryPointerIndex = virtual_address & mask;
}

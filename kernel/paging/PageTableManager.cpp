#include "PageTableManager.h"
#include "PagingUtils.h"

static PageTable* s_GlobalPageMapLevel4Table = nullptr;

PageTable* PageTableManager::GetGlobalPML4Table()
{
    if (s_GlobalPageMapLevel4Table)
        return s_GlobalPageMapLevel4Table;

    s_GlobalPageMapLevel4Table = reinterpret_cast<PageTable*>(PageFrameAllocator::GetGlobalAllocator().RequestPage());

    kSetMemory(s_GlobalPageMapLevel4Table, 0, PAGE_SIZE);
    return s_GlobalPageMapLevel4Table;
}

PageTableManager::PageTableManager(PageTable* PageMapLevel4Address)
{
    this->PageMapLevel4Table = PageMapLevel4Address;
}

void PageTableManager::MapMemory(void* pVirtual, void* pPhysical, MemoryPageAttribs attribs, MemoryPagePrivilegeLevel privilege_lvl)
{
    bool is_page_writable = (attribs == MemoryPageAttribs::READ_WRITE);
    bool is_page_usermode_level = (privilege_lvl == MemoryPagePrivilegeLevel::USERMODE);

    PageTableIndexer indexer((uint64_t)pVirtual);
    PageTableEntry entry;
    auto& Allocator = PageFrameAllocator::GetGlobalAllocator();

    // -------------------------------------------------------------------- //
    //                   PAGE DIRECTORY POINTER TABLE                       //
    // -------------------------------------------------------------------- //
    entry = PageMapLevel4Table->entries[indexer.PageDirectoryPointerIndex];
    PageTable* PageDirectoryPointerTable;
    if (!entry.GetFlag(PT_Flags::Present))
    {
        // If the page entry is not yet present, get a new page for it
        PageDirectoryPointerTable = reinterpret_cast<PageTable*>(Allocator.RequestPage());
        kSetMemory(PageDirectoryPointerTable, 0, PAGE_SIZE);

        entry.SetAddress((uint64_t)PageDirectoryPointerTable >> 12);
        entry.SetFlag(PT_Flags::Present, true);
        entry.SetFlag(PT_Flags::Writable, is_page_writable);
        entry.SetFlag(PT_Flags::Writable, is_page_usermode_level);
        PageMapLevel4Table->entries[indexer.PageDirectoryPointerIndex] = entry;
    }
    else
    {
        // If the page already exists
        PageDirectoryPointerTable = reinterpret_cast<PageTable*>(entry.GetAddress() << 12);
    }

    // -------------------------------------------------------------------- //
    //                        PAGE DIRECTORY TABLE                          //
    // -------------------------------------------------------------------- //
    entry = PageDirectoryPointerTable->entries[indexer.PageDirectoryIndex];
    PageTable* PageDirectoryTable;
    if (!entry.GetFlag(PT_Flags::Present))
    {
        // If the page entry is not yet present, get a new page for it
        PageDirectoryTable = reinterpret_cast<PageTable*>(Allocator.RequestPage());
        kSetMemory(PageDirectoryTable, 0, PAGE_SIZE);

        entry.SetAddress((uint64_t)PageDirectoryTable >> 12);
        entry.SetFlag(PT_Flags::Present, true);
        entry.SetFlag(PT_Flags::Writable, is_page_writable);
        entry.SetFlag(PT_Flags::Writable, is_page_usermode_level);
        PageDirectoryPointerTable->entries[indexer.PageDirectoryIndex] = entry;
    }
    else
    {
        // If the page already exists
        PageDirectoryTable = reinterpret_cast<PageTable*>(entry.GetAddress() << 12);
    }

    // -------------------------------------------------------------------- //
    //                            PAGE TABLE                                //
    // -------------------------------------------------------------------- //
    entry = PageDirectoryTable->entries[indexer.PageTableIndex];
    PageTable* _PageTable;
    if (!entry.GetFlag(PT_Flags::Present))
    {
        // If the page entry is not yet present, get a new page for it
        _PageTable = reinterpret_cast<PageTable*>(Allocator.RequestPage());
        kSetMemory(_PageTable, 0, PAGE_SIZE);

        entry.SetAddress((uint64_t)_PageTable >> 12);
        entry.SetFlag(PT_Flags::Present, true);
        entry.SetFlag(PT_Flags::Writable, is_page_writable);
        entry.SetFlag(PT_Flags::Writable, is_page_usermode_level);
        PageDirectoryTable->entries[indexer.PageIndex] = entry;
    }
    else
    {
        // If the page already exists
        _PageTable = reinterpret_cast<PageTable*>(entry.GetAddress() << 12);
    }

    // -------------------------------------------------------------------- //
    //                               PAGE                                   //
    // -------------------------------------------------------------------- //
    entry = _PageTable->entries[indexer.PageIndex];
    entry.SetAddress((uint64_t)pPhysical >> 12);
    entry.SetFlag(PT_Flags::Present, true);
    entry.SetFlag(PT_Flags::Writable, is_page_writable);
    entry.SetFlag(PT_Flags::Writable, is_page_usermode_level);
    _PageTable->entries[indexer.PageIndex] = entry;
}

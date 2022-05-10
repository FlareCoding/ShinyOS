#pragma once
#include "PageBitmap.h"
#include "Paging.h"
#include <kernel/MemoryMap.h>
#include <kernel/KernelMemory.h>

class PageFrameAllocator
{
public:
    static PageFrameAllocator& GetGlobalAllocator();

    PageFrameAllocator() = default;
    inline PageBitmap& GetPageBitmap() { return m_PageBitmap; }

    void ReadBIOSMemoryMap();

    void FreePage(void* address);
    void LockPage(void* address);
    void FreePages(void* address, uint64_t page_count);
    void LockPages(void* address, uint64_t page_count);

    void* RequestPage();

    uint64_t GetFreeRAM();
    uint64_t GetUsedRAM();
    uint64_t GetReservedRAM();

private:
    // Bitmap specifying which pages are in use and which
    // are free (true - in use, false - free / not in use).
    PageBitmap m_PageBitmap;

    void InitializePageBitmap(size_t size, void* buffer);

    void ReservePage(void* address);
    void ReleasePage(void* address);
    void ReservePages(void* address, uint64_t page_count);
    void ReleasePages(void* address, uint64_t page_count);

    uint64_t m_LastRequestedPageIndex = 0;
};

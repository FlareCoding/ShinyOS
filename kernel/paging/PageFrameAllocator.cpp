#include "PageFrameAllocator.h"

static uint64_t s_FreeMemory;
static uint64_t s_ReservedMemory;
static uint64_t s_UsedMemory;

static PageFrameAllocator s_GlobalPageFrameAllocator;

PageFrameAllocator& PageFrameAllocator::GetGlobalAllocator()
{
    return s_GlobalPageFrameAllocator;
}

void PageFrameAllocator::ReadBIOSMemoryMap()
{
    static bool s_IsInitialized = false;

    if (s_IsInitialized)
        return;

    // Find the largest region in the memory map
    MemoryMapEntry_t* LargestEntry = kGetUsableMemoryRegions()[0];

    for (uint8_t i = 0; i < _MemoryMapUsableMemoryRegions; i++)
    {
        MemoryMapEntry_t* Entry = kGetUsableMemoryRegions()[i];

        if (Entry->RegionSize > LargestEntry->RegionSize)
            LargestEntry = Entry;
    }

    // Calculate the amount of free available
    // memory and the page bitmap size.
    s_FreeMemory = kGetTotalMemorySize();
    uint64_t BitmapSize = s_FreeMemory / PAGE_SIZE / 8 + 1;

    // Initialize the page bitmap
    InitializePageBitmap(BitmapSize, (void*)LargestEntry->RegionBase);

    // Lock the pages with bitmap's buffer
    LockPages(m_PageBitmap.Buffer, m_PageBitmap.Size / PAGE_SIZE + 1);

    // Reserve pages of unusable/reserved memory
    for (uint8_t i = 0; i < _MemoryMapReservedMemoryRegions; i++)
    {
        MemoryMapEntry_t* Entry = kGetReservedMemoryRegions()[i];

        ReservePages((void*)Entry->RegionBase, Entry->RegionSize / PAGE_SIZE + 1);
    }

    s_IsInitialized = true;
}

void PageFrameAllocator::InitializePageBitmap(size_t size, void* buffer)
{
    m_PageBitmap.Size = size;
    m_PageBitmap.Buffer = (uint8_t*)buffer;

    kSetMemory(m_PageBitmap.Buffer, 0, m_PageBitmap.Size);
}

void PageFrameAllocator::FreePage(void* address)
{
    uint64_t bitmap_idx = (uint64_t)address / PAGE_SIZE;

    // Check if the memory is already free
    if (!m_PageBitmap[bitmap_idx])
        return;

    if (m_PageBitmap.Set(bitmap_idx, false))
    {
        s_FreeMemory += PAGE_SIZE;
        s_UsedMemory -= PAGE_SIZE;

        if (m_LastRequestedPageIndex > bitmap_idx)
            m_LastRequestedPageIndex = bitmap_idx;
    }
}

void PageFrameAllocator::LockPage(void* address)
{
    uint64_t bitmap_idx = (uint64_t)address / PAGE_SIZE;

    // Check if the memory is already in use
    if (m_PageBitmap[bitmap_idx])
        return;

    // Mark the 
    if (m_PageBitmap.Set(bitmap_idx, true))
    {
        s_FreeMemory -= PAGE_SIZE;
        s_UsedMemory += PAGE_SIZE;
    }
}

void PageFrameAllocator::ReservePage(void* address)
{
    uint64_t bitmap_idx = (uint64_t)address / PAGE_SIZE;

    // Check if the memory is already free
    if (m_PageBitmap[bitmap_idx])
        return;

    if (m_PageBitmap.Set(bitmap_idx, true))
    {
        s_FreeMemory -= PAGE_SIZE;
        s_ReservedMemory += PAGE_SIZE;
    }
}

void PageFrameAllocator::ReleasePage(void* address)
{
    uint64_t bitmap_idx = (uint64_t)address / PAGE_SIZE;

    // Check if the memory is already in use
    if (!m_PageBitmap[bitmap_idx])
        return;

    // Mark the 
    if (m_PageBitmap.Set(bitmap_idx, false))
    {
        s_FreeMemory += PAGE_SIZE;
        s_ReservedMemory -= PAGE_SIZE;

        if (m_LastRequestedPageIndex > bitmap_idx)
            m_LastRequestedPageIndex = bitmap_idx;
    }
}

void PageFrameAllocator::FreePages(void* address, uint64_t page_count)
{
    for (uint64_t i = 0; i < page_count; ++i)
        FreePage((void*)((uint64_t)address + (i * PAGE_SIZE)));
}

void PageFrameAllocator::LockPages(void* address, uint64_t page_count)
{
    for (uint64_t i = 0; i < page_count; ++i)
        LockPage((void*)((uint64_t)address + (i * PAGE_SIZE)));
}

void PageFrameAllocator::ReservePages(void* address, uint64_t page_count)
{
    for (uint64_t i = 0; i < page_count; ++i)
        ReservePage((void*)((uint64_t)address + (i * PAGE_SIZE)));
}

void PageFrameAllocator::ReleasePages(void* address, uint64_t page_count)
{
    for (uint64_t i = 0; i < page_count; ++i)
        ReleasePage((void*)((uint64_t)address + (i * PAGE_SIZE)));
}

void* PageFrameAllocator::RequestPage()
{
    for (uint64_t idx = m_LastRequestedPageIndex; idx < m_PageBitmap.Size * 8; ++idx)
    {
        // Check if the page is used, if so, move on to the next one
        if (m_PageBitmap[idx])
            continue;

        void* page_address = (void*)(idx * PAGE_SIZE);
        LockPage(page_address);

        m_LastRequestedPageIndex = idx;
        return page_address;
    }

    // At this point, page fram swap is required
    return nullptr;
}

uint64_t PageFrameAllocator::GetFreeRAM()
{
    return s_FreeMemory;
}

uint64_t PageFrameAllocator::GetUsedRAM()
{
    return s_UsedMemory;
}

uint64_t PageFrameAllocator::GetReservedRAM()
{
    return s_ReservedMemory;
}

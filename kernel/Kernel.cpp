#include <drivers/VGAScreenDriver.h>
#include <drivers/KeyboardDriver.h>
#include "SystemTimer.h"
#include "paging/PageTableManager.h"

#define KERNEL_INFINITE_LOOP while(true)

extern uint64_t _KernelAddressStart;
extern uint64_t _KernelAddressEnd;

// Handles keyboard events such KeyDown or KeyUp
void _KeyboardEventCallbackHandler(KbdEvent_t evt);

// Sets up the IDT and registers appropriate ISR and IRQ handlers
void _kSetupInterrupts();

// Creates page tables based on the BIOS memory
// map and locks the pages used by the kernel.
void _kSetupPaging();

// Initializes all the kernel utilities and resources
void _kInitializeKernel();

// Kernel's Entry Point
extern "C" void _kmain() {
    // Initialize resources and utilities
    _kInitializeKernel();

    PageTableManager PTManager(PageTableManager::GetGlobalPML4Table());

    PTManager.MapMemory((void*)0x600000000, (void*)0x80000, MemoryPageAttribs::READ_WRITE, MemoryPagePrivilegeLevel::KERNEL);

    uint64_t* test = (uint64_t*)0x600000000;
    *test = 4554;
    kPrintInt(*test); kPrint("\n");

    KERNEL_INFINITE_LOOP;
}

void _KeyboardEventCallbackHandler(KbdEvent_t evt)
{
    if (evt.EventType != KEYBOARD_EVENT_KEYPRESS)
        return;

    if (evt.CharValue)
    {
        kPrintChar(evt.CharValue, -1, -1, VGA_DEFAULT_COLOR);
    }
    else
    {
        switch (evt.ScanCode)
        {
        case KEYCODE_BACKSPACE:
        {
            kBackspace(1);
            break;
        }
        case KEYCODE_RETURN:
        {
            kPrint("\n");
            break;
        }
        default:
            break;
        }
    }
}

void _kSetupInterrupts()
{
    // Initialize IDT and setup interrupt service routines
    kInitializeIDT();

    // Setup user IRQ handlers
    kRegisterUserIrqHandler(IRQ0, kHandleSystemClockInterrupt);
    kRegisterUserIrqHandler(IRQ1, kHandleKeyboardInterrupt);
}

void _kSetupPaging()
{
    auto& GlobalPageAllocator = PageFrameAllocator::GetGlobalAllocator();
    GlobalPageAllocator.ReadBIOSMemoryMap();

    // Lock the pages of low addresses before the kernel
    // to prevent messing with bootsector allocated memory.
    uint64_t LowAddressesSize       = (uint64_t)&_KernelAddressStart - 0x0000;
    uint64_t LowAddressesPageCount  = LowAddressesSize / PAGE_SIZE + 1;

    GlobalPageAllocator.LockPages(0x0000, LowAddressesPageCount);

    // Lock the pages used by the kernel
    uint64_t KernelSize         = (uint64_t)&_KernelAddressEnd - (uint64_t)&_KernelAddressStart;
    uint64_t KernelPageCount    = KernelSize / PAGE_SIZE + 1;

    GlobalPageAllocator.LockPages(&_KernelAddressStart, KernelPageCount);

    // Lock the pages used by the BIOS Memory Map
    uint64_t MemoryMapSize      = _MemoryMapAvailableMemoryRegions * sizeof(MemoryMapEntry_t);
    uint64_t MemoryMapPageCount = MemoryMapSize / PAGE_SIZE + 1;

    GlobalPageAllocator.LockPages((void*)MEMORY_MAP_ADDRESS, MemoryMapPageCount);

    // Setup a local page table manager to perform identity mapping
    auto GlobalPML4Table = PageTableManager::GetGlobalPML4Table();
    PageTableManager PTManager(GlobalPML4Table);

    // Identity mapping every virtual address to the same physical address
    for (uint64_t i = 0; i < 0x200000; i += PAGE_SIZE)
    {
        PTManager.MapMemory((void*)i, (void*)i, MemoryPageAttribs::READ_WRITE, MemoryPagePrivilegeLevel::KERNEL);
    }

    for (uint64_t i = VGA_MEMORY_ADDRESS; i < VGA_MEMORY_ADDRESS + VGA_SCREEN_MAX_CURSOR_INDEX * 2; i += PAGE_SIZE)
    {
        PTManager.MapMemory((void*)i, (void*)i, MemoryPageAttribs::READ_WRITE, MemoryPagePrivilegeLevel::KERNEL);
    }

    asm volatile("mov %0, %%cr3" : : "r"(GlobalPML4Table));
}

void _kInitializeKernel()
{
    // Clear the screen
    kClearVGAScreenBuffer();

    // Calculate the number of memory
    // regions available to the kernel.
    kCalculateMemoryMapRegions();

    // Setup IDT and interrupt handlers
    _kSetupInterrupts();

    // Setup paging and lock the pages used by the kernel
    _kSetupPaging();

    // Register a kernel key event callback
    kKeyboardDriverRegisterKeyEventCallback(_KeyboardEventCallbackHandler);
}

#include <drivers/VGAScreenDriver.h>
#include <drivers/KeyboardDriver.h>
#include "SystemTimer.h"
#include "MemoryMap.h"
#include "MemoryHeap.h"

void _KeyboardEventCallbackHandler(KbdEvent_t evt);

void _kmain() {
    // Calculate the number of memory
    // regions available to the kernel.
    kCalculateMemoryMapRegions();

    // Initialize IDT and setup interrupt service routines
    kInitializeIDT();

    // Setup user IRQ handlers
    kRegisterUserIrqHandler(IRQ0, kHandleSystemClockInterrupt);
    kRegisterUserIrqHandler(IRQ1, kHandleKeyboardInterrupt);

    // Register a kernel key event callback
    kKeyboardDriverRegisterKeyEventCallback(_KeyboardEventCallbackHandler);

    kClearVGAScreenBuffer();
    kPrint("===== Memory Map =====\n");
    for (uint8_t i = 0; i < _MemoryMapUsableMemoryRegions; i++)
    {
        MemoryMapEntry_t* Entry = kGetUsableMemoryRegions()[i];

        kPrint("Region "); kPrintInt(i + 1); kPrint("\n");
        kPrint("    Base     : "); kPrintHex(Entry->RegionBase); kPrint("\n");
        kPrint("    Size     : "); kPrintHex(Entry->RegionSize); kPrint("\n");
        kPrint("    Type     : "); kPrintInt(Entry->RegionType); kPrint("\n");
        kPrint("    Attribs  : "); kPrintHex(Entry->ExtendedAttribs); kPrint("\n\n");
    }

    MemoryHeap_t heap = kCreateHeap(0x100000, 0x10000);

    void* test_addy = kMemoryAlloc(&heap, 0x10);
    kPrintHex((uint64_t)test_addy); kPrint("\n");

    void* test_addy2 = kMemoryAlloc(&heap, 0x10);
    kPrintHex((uint64_t)test_addy2); kPrint("\n");

    void* test_addy3 = kMemoryAlloc(&heap, 0x10);
    kPrintHex((uint64_t)test_addy3); kPrint("\n");

    //kFreeMemory(&heap, test_addy);
    kFreeMemory(&heap, test_addy2);
    //kFreeMemory(&heap, test_addy3);

    void* test_addy4 = kMemoryAlloc(&heap, 0x10);
    kPrintHex((uint64_t)test_addy4); kPrint("\n");
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


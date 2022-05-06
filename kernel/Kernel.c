#include <drivers/VGAScreenDriver.h>
#include <drivers/KeyboardDriver.h>
#include "SystemTimer.h"

void _KeyboardEventCallbackHandler(KbdEvent_t evt);

void _kmain() {
    // Initialize IDT and setup interrupt service routines
    kInitializeIDT();

    // Setup user IRQ handlers
    kRegisterUserIrqHandler(IRQ0, kHandleSystemClockInterrupt);
    kRegisterUserIrqHandler(IRQ1, kHandleKeyboardInterrupt);

    // Register a kernel key event callback
    kKeyboardDriverRegisterKeyEventCallback(_KeyboardEventCallbackHandler);

    kClearVGAScreenBuffer();
    kPrintHex((uint64_t)&_kmain);
    kPrint("\n");
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


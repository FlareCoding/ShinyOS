#include <drivers/VGAScreenDriver.h>
#include <drivers/KeyboardDriver.h>
#include "Idt.h"

int g_ShiftPressed = 0;

void _keyboard_event_callback_handler(KbdEvent_t evt);

void _kmain() {
    // Initialize IDT and setup interrupt service routines
    kInitializeIDT();

    // Setup keyboard interrupt handler
    kRegisterUserIrqHandler(IRQ1, kHandleKeyboardInterrupt);

    // Register a kernel key event callback
    kKeyboardDriverRegisterKeyEventCallback(_keyboard_event_callback_handler);

    kClearVGAScreenBuffer();
    kPrintHex((uint64_t)&_kmain);
    kPrint("\n");
}

void _keyboard_event_callback_handler(KbdEvent_t evt)
{
    if (evt.EventType != KEYBOARD_EVENT_KEYPRESS)
        return;

    if (evt.CharValue)
    {
        if (g_ShiftPressed)
            evt.CharValue -= 32;

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


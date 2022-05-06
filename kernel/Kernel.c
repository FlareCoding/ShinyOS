#include <drivers/VGAScreenDriver.h>
#include <drivers/KeyboardDriver.h>
#include "Idt.h"

int g_ShiftPressed = 0;

void _keyboard_event_callback_handler(uint8_t scan_code, uint8_t chr);

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

void _keyboard_event_callback_handler(uint8_t scan_code, uint8_t chr)
{
    if (chr)
    {
        if (g_ShiftPressed)
            chr -= 32;

        kPrintChar(chr, -1, -1, VGA_DEFAULT_COLOR);
    }
    else
    {
        switch (scan_code)
        {
        case OHNOS_KEYCODE_BACKSPACE:
        {
            kBackspace(1);
            break;
        }
        case OHNOS_KEYCODE_LSHIFT_PRESSED:
        {
            g_ShiftPressed = 1;
            break; 
        }
        case OHNOS_KEYCODE_LSHIFT_RELEASED:
        {
            g_ShiftPressed = 0;
            break;
        }
        case OHNOS_KEYCODE_RSHIFT_PRESSED:
        {
            g_ShiftPressed = 1;
            break;
        }
        case OHNOS_KEYCODE_RSHIFT_RELEASED:
        {
            g_ShiftPressed = 0;
            break;
        }
        case OHNOS_KEYCODE_RETURN:
        {
            kPrint("\n");
            break;
        }
        default:
            break;
        }
    }
}


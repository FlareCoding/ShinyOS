#include "KeyboardDriver.h"
#include "VGAScreenDriver.h"

const char g_KeyboardScanCodes[58] = {
    0, 0,
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', 0, 0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '[', ']', 0, 0, 
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0, '*', 0, ' '
};

static keyboard_event_callback_t g_KeyboardEventCallback = 0;

void kSetKeyboardEventCallback(keyboard_event_callback_t callback)
{
    g_KeyboardEventCallback = callback;
}

void kHandleKeyboardInterrupt()
{
    uint8_t scan_code = kPortByteIn(0x60);
    uint8_t chr = 0;

    if (scan_code < 0x3A)
        chr = g_KeyboardScanCodes[scan_code];

    if (g_KeyboardEventCallback != 0)
        g_KeyboardEventCallback(scan_code, chr);

    // Specify that we finished handling the interrupt.
    kPortByteOut(0x20, 0x20);
    kPortByteOut(0xA0, 0x20);
}

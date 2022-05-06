#include "KeyboardDriver.h"
#include "VGAScreenDriver.h"

KernelKeyEventCallback_t g_KeyEventCallback = 0;

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

void kKeyboardDriverRegisterKeyEventCallback(KernelKeyEventCallback_t callback)
{
    g_KeyEventCallback = callback;
}

void kHandleKeyboardInterrupt(NakedInterruptFrame_t* frame, uint8_t int_no)
{
    uint8_t scan_code = kPortByteIn(0x60);
    uint8_t chr = 0;
    int is_release = scan_code >= 0x80;

    if (is_release)
        scan_code -= 0x80;

    if (scan_code < 0x3A)
        chr = g_KeyboardScanCodes[scan_code];

    KbdEvent_t kbd_event = { 0 };
    kbd_event.EventType = is_release ? KEYBOARD_EVENT_KEYRELEASE : KEYBOARD_EVENT_KEYPRESS;
    kbd_event.ScanCode = scan_code;
    kbd_event.CharValue = chr;

    if (g_KeyEventCallback != 0)
        g_KeyEventCallback(kbd_event);
}

#include "KeyboardDriver.h"
#include "VGAScreenDriver.h"

KernelKeyEventCallback_t g_KeyEventCallback = 0;

int g_IsCapsLockOn = 0;
int g_IsShiftPressed = 0;

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

// Accounts for the presence of shift or capslock pressed
// and converts the character to its uppercase equivalent.
uint8_t s_kShiftNormalizeChar(uint8_t chr, uint8_t shift_pressed, uint8_t capslock_on)
{
    // ShiftOn XOR CapsLockOn
    if (!shift_pressed != !capslock_on)
        if (chr >= 'a' && chr <= 'z')
            return chr - 32;

    if (!shift_pressed)
        return chr;

    switch (chr)
    {
    case '`': return '~';
    case '1': return '!';
    case '2': return '@';
    case '3': return '#';
    case '4': return '$';
    case '5': return '%';
    case '6': return '^';
    case '7': return '&';
    case '8': return '*';
    case '9': return '(';
    case '0': return ')';
    case '-': return '_';
    case '=': return '+';
    case ',': return '<';
    case '.': return '>';
    case '/': return '?';
    case ';': return ':';
    case '\'': return '"';
    case '[': return '{';
    case ']': return '}';
    case '\\': return '|';
    default: return chr;
    }
}

void kKeyboardDriverRegisterKeyEventCallback(KernelKeyEventCallback_t callback)
{
    g_KeyEventCallback = callback;
}

void kHandleKeyboardInterrupt(NakedInterruptFrame_t* frame, uint8_t int_no)
{
    // Get the scan code from the keyboard IO port
    uint8_t scan_code = kPortByteIn(0x60);
    uint8_t chr = 0;

    // If the high order bit is set, then it's
    // an indicator that a key has been released.
    int is_release = scan_code >= 0x80;

    // Normalize the scan code to account for key release flag
    if (is_release)
        scan_code -= 0x80;

    // Retrieve the character value for the scan code
    if (scan_code < 0x3A)
        chr = g_KeyboardScanCodes[scan_code];

    // Perform internal shift and CAPS lock checks
    if (scan_code == KEYCODE_LSHIFT || scan_code == KEYCODE_RSHIFT)
        g_IsShiftPressed = !is_release;

    if (scan_code == KEYCODE_CAPSLOCK && is_release)
        g_IsCapsLockOn = !g_IsCapsLockOn;

    // Adjust the character value
    // according to shift and capslock states.
    if (g_IsShiftPressed || g_IsCapsLockOn)
        chr = s_kShiftNormalizeChar(chr, g_IsShiftPressed, g_IsCapsLockOn);

    // Construct the keyboard event structure
    KbdEvent_t kbd_event = { 0 };
    kbd_event.EventType = is_release ? KEYBOARD_EVENT_KEYRELEASE : KEYBOARD_EVENT_KEYPRESS;
    kbd_event.ScanCode = scan_code;
    kbd_event.CharValue = chr;

    // Call the event callback if one exists
    if (g_KeyEventCallback != 0)
        g_KeyEventCallback(kbd_event);
}

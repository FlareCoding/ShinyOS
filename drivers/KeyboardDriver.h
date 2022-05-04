#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H
#include <kernel/typedefs.h>

#define OHNOS_KEYCODE_BACKSPACE         0x8E
#define OHNOS_KEYCODE_LSHIFT_PRESSED    0x2A
#define OHNOS_KEYCODE_LSHIFT_RELEASED   0xAA
#define OHNOS_KEYCODE_RSHIFT_PRESSED    0x36
#define OHNOS_KEYCODE_RSHIFT_RELEASED   0xB6
#define OHNOS_KEYCODE_RETURN            0x9C

typedef void(*keyboard_event_callback_t)(uint8_t scan_code, uint8_t chr);

// Called by the interrupt service routine
void kHandleKeyboardInterrupt();

// Kernel callback function to handle a keypress
void kSetKeyboardEventCallback(keyboard_event_callback_t callback);

#endif

#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H
#include <kernel/Idt.h>

#define KEYCODE_BACKSPACE   0xE
#define KEYCODE_LSHIFT      0x2A
#define KEYCODE_RSHIFT      0x36
#define KEYCODE_RETURN      0x1C

#define KEYBOARD_EVENT_KEYPRESS   1
#define KEYBOARD_EVENT_KEYRELEASE 2

typedef struct
{
    uint8_t EventType;
    uint8_t ScanCode;
    uint8_t CharValue;
} KbdEvent_t;

// Called by the interrupt service routine
void kHandleKeyboardInterrupt(NakedInterruptFrame_t* frame, uint8_t int_no);

// Kernel callback to a keypress
typedef void(*KernelKeyEventCallback_t)(KbdEvent_t);
void kKeyboardDriverRegisterKeyEventCallback(KernelKeyEventCallback_t callback);

#endif

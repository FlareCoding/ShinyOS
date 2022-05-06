#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H
#include <kernel/Idt.h>

#define OHNOS_KEYCODE_BACKSPACE         0x8E
#define OHNOS_KEYCODE_LSHIFT_PRESSED    0x2A
#define OHNOS_KEYCODE_LSHIFT_RELEASED   0xAA
#define OHNOS_KEYCODE_RSHIFT_PRESSED    0x36
#define OHNOS_KEYCODE_RSHIFT_RELEASED   0xB6
#define OHNOS_KEYCODE_RETURN            0x9C

// Called by the interrupt service routine
void kHandleKeyboardInterrupt(NakedInterruptFrame_t* frame, uint8_t int_no);

// Kernel callback to a keypress
typedef void(*KernelKeyEventCallback_t)(uint8_t scan_code, uint8_t c);
void kKeyboardDriverRegisterKeyEventCallback(KernelKeyEventCallback_t callback);

#endif

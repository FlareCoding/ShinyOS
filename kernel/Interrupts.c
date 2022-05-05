#include "Interrupts.h"
#include <drivers/VGAScreenDriver.h>
#include <drivers/KeyboardDriver.h>

void kSetInterruptServiceRoutine(uint8_t isr_index, void* isr, uint8_t flags)
{
    IdtEntry_t* descriptor = &g_IDT[isr_index];

    g_IDT[isr_index].isr_low        = (uint16_t)(((uint64_t)isr & 0x000000000000ffff));
    g_IDT[isr_index].kernel_cs      = 0x08;
    g_IDT[isr_index].ist            = 0;
    g_IDT[isr_index].attributes     = flags;
    g_IDT[isr_index].isr_mid        = (uint16_t)(((uint64_t)isr & 0x00000000ffff0000) >> 16);
    g_IDT[isr_index].isr_high       = (uint32_t)(((uint64_t)isr & 0xffffffff00000000) >> 32);
    g_IDT[isr_index].reserved       = 0;
}

void kInitializeIDT()
{
    g_idtr.size    = (uint16_t)(sizeof(IdtEntry_t) * 256);
    g_idtr.address = (uint64_t)&g_IDT[0];

    // Setup CPU exception handlers
    // kSetInterruptServiceRoutine(0,  kISR0,  TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(1,  kISR1,  TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(2,  kISR2,  TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(3,  kISR3,  TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(4,  kISR4,  TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(5,  kISR5,  TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(6,  kISR6,  TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(7,  kISR7,  TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(8,  kISR8,  TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(9,  kISR9,  TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(10, kISR10, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(11, kISR11, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(12, kISR12, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(13, kISR13, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(14, kISR14, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(15, kISR15, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(16, kISR16, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(17, kISR17, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(18, kISR18, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(19, kISR19, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(20, kISR20, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(21, kISR21, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(22, kISR22, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(23, kISR23, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(24, kISR24, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(25, kISR25, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(26, kISR26, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(27, kISR27, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(28, kISR28, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(29, kISR29, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(30, kISR30, TRAP_GATE_FLAGS);
    // kSetInterruptServiceRoutine(31, kISR31, TRAP_GATE_FLAGS);

    // Remap the PIC chip
    // kPortByteOut(0x20, 0x11);
    // kPortByteOut(0xA0, 0x11);
    // kPortByteOut(0x21, 0x20);
    // kPortByteOut(0xA1, 0x28);
    // kPortByteOut(0x21, 0x04);
    // kPortByteOut(0xA1, 0x02);
    // kPortByteOut(0x21, 0x01);
    // kPortByteOut(0xA1, 0x01);
    // kPortByteOut(0x21, 0x0);
    // kPortByteOut(0xA1, 0x0);

    // Setup IRQ handlers
    // kSetInterruptServiceRoutine(IRQ0,  kIRQ0,  INT_GATE_FLAGS);
    // kSetInterruptServiceRoutine(IRQ1,  kIRQ1,  INT_GATE_FLAGS);
    // kSetInterruptServiceRoutine(IRQ2,  kIRQ2,  INT_GATE_FLAGS);
    // kSetInterruptServiceRoutine(IRQ3,  kIRQ3,  INT_GATE_FLAGS);
    // kSetInterruptServiceRoutine(IRQ4,  kIRQ4,  INT_GATE_FLAGS);
    // kSetInterruptServiceRoutine(IRQ5,  kIRQ5,  INT_GATE_FLAGS);
    // kSetInterruptServiceRoutine(IRQ6,  kIRQ6,  INT_GATE_FLAGS);
    // kSetInterruptServiceRoutine(IRQ7,  kIRQ7,  INT_GATE_FLAGS);
    // kSetInterruptServiceRoutine(IRQ8,  kIRQ8,  INT_GATE_FLAGS);
    // kSetInterruptServiceRoutine(IRQ9,  kIRQ9,  INT_GATE_FLAGS);
    // kSetInterruptServiceRoutine(IRQ10, kIRQ10, INT_GATE_FLAGS);
    // kSetInterruptServiceRoutine(IRQ11, kIRQ11, INT_GATE_FLAGS);
    // kSetInterruptServiceRoutine(IRQ12, kIRQ12, INT_GATE_FLAGS);
    // kSetInterruptServiceRoutine(IRQ13, kIRQ13, INT_GATE_FLAGS);
    // kSetInterruptServiceRoutine(IRQ14, kIRQ14, INT_GATE_FLAGS);
    // kSetInterruptServiceRoutine(IRQ15, kIRQ15, INT_GATE_FLAGS);

    // Setup specific individual routines
    kSetInterruptServiceRoutine(1, kKeyboardISR, INT_GATE_FLAGS);

    kRemapPIC();

    kPortByteOut(0x21, 0xFD);
    kPortByteOut(0xA1, 0xFF);

    __asm__ __volatile__ ("lidt %0" : : "memory"(g_idtr));
    __asm__ __volatile__ ("sti");
}

void kInterruptRequestHandler(InterruptFrame_t* frame, uint32_t intno)
{
    //kHandleKeyboardInterrupt();
    kPrintHex(intno);
    kPrint("\n");
    //kPortByteIn(0x60);

    // Specify that we finished handling the interrupt.
    //kPortByteOut(0x20, 0x20);
    //kPortByteOut(0xA0, 0x20);
}

void kKeyboardISR(InterruptFrame_t* interrupt_frame)
{
    kHandleKeyboardInterrupt();

    // Specify that we finished handling the interrupt.
    kPortByteOut(0x20, 0x20);
    kPortByteOut(0xA0, 0x20);
}

// ================================================================== //
//                        CPU Exception Handlers                      //
// ================================================================== //

void kISR0(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 0 occured -----\n");
}

void kISR1(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 1 occured -----\n");
}

void kISR2(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 2 occured -----\n");
}

void kISR3(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 3 occured -----\n");
}

void kISR4(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 4 occured -----\n");
}

void kISR5(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 5 occured -----\n");
}

void kISR6(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 6 occured -----\n");
}

void kISR7(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 7 occured -----\n");
}

void kISR8(InterruptFrame_t* frame, interrupt_error_code_t errno)
{
    kPrint("----- CPU Exception 8 occured -----\n");
}

void kISR9(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 9 occured -----\n");
}

void kISR10(InterruptFrame_t* frame, interrupt_error_code_t errno)
{
    kPrint("----- CPU Exception 10 occured -----\n");
}

void kISR11(InterruptFrame_t* frame, interrupt_error_code_t errno)
{
    kPrint("----- CPU Exception 11 occured -----\n");
}

void kISR12(InterruptFrame_t* frame, interrupt_error_code_t errno)
{
    kPrint("----- CPU Exception 12 occured -----\n");
}

void kISR13(InterruptFrame_t* frame, interrupt_error_code_t errno)
{
    kPrint("----- CPU Exception 13 occured -----\n");
}

void kISR14(InterruptFrame_t* frame, interrupt_error_code_t errno)
{
    kPrint("----- CPU Exception 14 occured -----\n");
}

void kISR15(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 15 occured -----\n");
}

void kISR16(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 16 occured -----\n");
}

void kISR17(InterruptFrame_t* frame, interrupt_error_code_t errno)
{
    kPrint("----- CPU Exception 17 occured -----\n");
}

void kISR18(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 18 occured -----\n");
}

void kISR19(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 19 occured -----\n");
}

void kISR20(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 20 occured -----\n");
}

void kISR21(InterruptFrame_t* frame, interrupt_error_code_t errno)
{
    kPrint("----- CPU Exception 21 occured -----\n");
}

void kISR22(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 22 occured -----\n");
}

void kISR23(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 23 occured -----\n");
}

void kISR24(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 24 occured -----\n");
}

void kISR25(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 25 occured -----\n");
}

void kISR26(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 26 occured -----\n");
}

void kISR27(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 27 occured -----\n");
}

void kISR28(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 28 occured -----\n");
}

void kISR29(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 29 occured -----\n");
}

void kISR30(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 30 occured -----\n");
}

void kISR31(InterruptFrame_t* frame)
{
    kPrint("----- CPU Exception 31 occured -----\n");
}

// ================================================================== //
//                          IRQ HANDLERS                              //
// ================================================================== //

void kIRQ0(InterruptFrame_t* frame) { kInterruptRequestHandler(frame, 0); }
void kIRQ1(InterruptFrame_t* frame) { kInterruptRequestHandler(frame, 1); }
void kIRQ2(InterruptFrame_t* frame) { kInterruptRequestHandler(frame, 2); }
void kIRQ3(InterruptFrame_t* frame) { kInterruptRequestHandler(frame, 3); }
void kIRQ4(InterruptFrame_t* frame) { kInterruptRequestHandler(frame, 4); }
void kIRQ5(InterruptFrame_t* frame) { kInterruptRequestHandler(frame, 5); }
void kIRQ6(InterruptFrame_t* frame) { kInterruptRequestHandler(frame, 6); }
void kIRQ7(InterruptFrame_t* frame) { kInterruptRequestHandler(frame, 7); }
void kIRQ8(InterruptFrame_t* frame) { kInterruptRequestHandler(frame, 8); }
void kIRQ9(InterruptFrame_t* frame) { kInterruptRequestHandler(frame, 9); }
void kIRQ10(InterruptFrame_t* frame) { kInterruptRequestHandler(frame, 10); }
void kIRQ11(InterruptFrame_t* frame) { kInterruptRequestHandler(frame, 11); }
void kIRQ12(InterruptFrame_t* frame) { kInterruptRequestHandler(frame, 12); }
void kIRQ13(InterruptFrame_t* frame) { kInterruptRequestHandler(frame, 13); }
void kIRQ14(InterruptFrame_t* frame) { kInterruptRequestHandler(frame, 14); }
void kIRQ15(InterruptFrame_t* frame) { kInterruptRequestHandler(frame, 15); }

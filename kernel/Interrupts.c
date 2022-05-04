#include "Interrupts.h"
#include <drivers/VGAScreenDriver.h>
#include <drivers/KeyboardDriver.h>

// Service routines are located in the ohnos_idt.asm
extern void* ohnos_interrupt_service_routine_keyboard;

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

    // Setup specific individual routines
    kSetInterruptServiceRoutine(1, kKeyboardISR, INT_GATE_FLAGS);

    kRemapPIC();

    kPortByteOut(0x21, 0xFD);
    kPortByteOut(0xA1, 0xFF);

    __asm__ __volatile__ ("lidt %0" : : "memory"(g_idtr));
    __asm__ __volatile__ ("sti");
}

void kKeyboardISR(InterruptFrame_t* interrupt_frame)
{
    kHandleKeyboardInterrupt();
}

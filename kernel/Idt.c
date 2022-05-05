#include "Idt.h"
#include <drivers/VGAScreenDriver.h>

/* To print the message which defines every exception */
char* g_ExceptionMessages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void kKernelIsrHandler(InterruptFrame_t frame) {
    kPrint("Received interrupt: ");
    kPrintInt(frame.int_no);
    kPrint("\n");
    kPrint("Error code: ");
    kPrintHex(frame.err_no);
    kPrint("\n");
    kPrint(g_ExceptionMessages[frame.int_no]); kPrint("\n");
    kPrint("\n\n");

    __asm__ __volatile__ ("hlt");
}

void kSetIsrHandler(uint8_t isr_index, void* isr, uint8_t flags)
{
    IdtEntry_t* descriptor = &g_idt[isr_index];

    descriptor->isr_low         = (uint16_t)(((uint64_t)isr & 0x000000000000ffff));
    descriptor->kernel_cs       = 0x08;
    descriptor->ist             = 0;
    descriptor->flags           = flags;
    descriptor->isr_mid         = (uint16_t)(((uint64_t)isr & 0x00000000ffff0000) >> 16);
    descriptor->isr_high        = (uint32_t)(((uint64_t)isr & 0xffffffff00000000) >> 32);
    descriptor->reserved        = 0;
}

void kInitializeIDT()
{
    g_idtr.size    = (uint16_t)(sizeof(IdtEntry_t) * 256 - 1);
    g_idtr.address = (uint64_t)&g_idt;

    kSetIsrHandler(0, isr0, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(1, isr1, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(2, isr2, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(3, isr3, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(4, isr4, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(5, isr5, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(6, isr6, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(7, isr7, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(8, isr8, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(9, isr9, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(10, isr10, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(11, isr11, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(12, isr12, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(13, isr13, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(14, isr14, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(15, isr15, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(16, isr16, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(17, isr17, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(18, isr18, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(19, isr19, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(20, isr20, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(21, isr21, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(22, isr22, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(23, isr23, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(24, isr24, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(25, isr25, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(26, isr26, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(27, isr27, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(28, isr28, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(29, isr29, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(30, isr30, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(31, isr31, INTERRUPT_GATE_FLAGS);

    __asm__ __volatile__ ("lidt %0" : : "memory"(g_idtr));
    __asm__ __volatile__ ("sti");
}

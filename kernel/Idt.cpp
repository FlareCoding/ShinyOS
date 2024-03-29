#include "Idt.h"
#include <drivers/VGAScreenDriver.h>

UserIrqHandler_t g_UserIrqHandlers[16];

IdtEntry_t g_idt[256];
Idtr_t g_idtr;

/* To print the message which defines every exception */
const char* g_CPUExceptionMessages[] = {
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

void kSetIsrHandler(uint8_t isr_index, void* isr, uint8_t flags)
{
    IdtEntry_t* descriptor      = &g_idt[isr_index];

    descriptor->isr_low         = (uint16_t)(((uint64_t)isr & 0x000000000000ffff));
    descriptor->kernel_cs       = 0x08;
    descriptor->ist             = 0;
    descriptor->flags           = flags;
    descriptor->isr_mid         = (uint16_t)(((uint64_t)isr & 0x00000000ffff0000) >> 16);
    descriptor->isr_high        = (uint32_t)(((uint64_t)isr & 0xffffffff00000000) >> 32);
    descriptor->reserved        = 0;
}

void kKernelIsrHandler(InterruptFrame_t frame) {
    kPrint("Received exception interrupt: ");
    kPrintInt(frame.int_no);
    kPrint("\n");
    kPrint("Error code: ");
    kPrintHex(frame.err_no);
    kPrint("\n");
    kPrint(g_CPUExceptionMessages[frame.int_no]); kPrint("\n");
    kPrint("\n-------- registers --------\n");

    kPrint("cs     : "); kPrintHex(frame.cs); kPrint("\n");
    kPrint("ss     : "); kPrintHex(frame.ss); kPrint("\n");
    kPrint("ds     : "); kPrintHex(frame.ds); kPrint("\n");
    kPrint("rip    : "); kPrintHex(frame.rip); kPrint("\n");
    kPrint("rbp    : "); kPrintHex(frame.rbp); kPrint("\n");
    kPrint("rsp    : "); kPrintHex(frame.rsp); kPrint("\n");
    kPrint("eflags : "); kPrintHex(frame.eflags); kPrint("\n");

    asm volatile("hlt");
}

void kKernelIrqHandler(NakedInterruptFrame_t* frame, uint8_t int_no)
{
    // Call the user handler if one exists
    if (g_UserIrqHandlers[int_no - IRQ0] != 0) {
        UserIrqHandler_t user_handler = g_UserIrqHandlers[int_no - IRQ0];
        user_handler(frame, int_no);
    }

    // Tell the master and slave PICs
    // that interrupt has been processed.
    if (int_no >= IRQ8)
        kPortByteOut(SLAVE_PIC_COMMAND, 0x20);

    kPortByteOut(MASTER_PIC_COMMAND, 0x20);
}

void kRegisterUserIrqHandler(uint8_t entry, UserIrqHandler_t handler)
{
    g_UserIrqHandlers[entry - IRQ0] = handler;
}

void kIrq0(NakedInterruptFrame_t*  frame) { kKernelIrqHandler(frame, IRQ0);  }
void kIrq1(NakedInterruptFrame_t*  frame) { kKernelIrqHandler(frame, IRQ1);  }
void kIrq2(NakedInterruptFrame_t*  frame) { kKernelIrqHandler(frame, IRQ2);  }
void kIrq3(NakedInterruptFrame_t*  frame) { kKernelIrqHandler(frame, IRQ3);  }
void kIrq4(NakedInterruptFrame_t*  frame) { kKernelIrqHandler(frame, IRQ4);  }
void kIrq5(NakedInterruptFrame_t*  frame) { kKernelIrqHandler(frame, IRQ5);  }
void kIrq6(NakedInterruptFrame_t*  frame) { kKernelIrqHandler(frame, IRQ6);  }
void kIrq7(NakedInterruptFrame_t*  frame) { kKernelIrqHandler(frame, IRQ7);  }
void kIrq8(NakedInterruptFrame_t*  frame) { kKernelIrqHandler(frame, IRQ8);  }
void kIrq9(NakedInterruptFrame_t*  frame) { kKernelIrqHandler(frame, IRQ9);  }
void kIrq10(NakedInterruptFrame_t* frame) { kKernelIrqHandler(frame, IRQ10); }
void kIrq11(NakedInterruptFrame_t* frame) { kKernelIrqHandler(frame, IRQ11); }
void kIrq12(NakedInterruptFrame_t* frame) { kKernelIrqHandler(frame, IRQ12); }
void kIrq13(NakedInterruptFrame_t* frame) { kKernelIrqHandler(frame, IRQ13); }
void kIrq14(NakedInterruptFrame_t* frame) { kKernelIrqHandler(frame, IRQ14); }
void kIrq15(NakedInterruptFrame_t* frame) { kKernelIrqHandler(frame, IRQ15); }

void kInitializeIDT()
{
    g_idtr.size    = (uint16_t)(sizeof(IdtEntry_t) * 256 - 1);
    g_idtr.address = (uint64_t)&g_idt;

    kSetIsrHandler(0,  (void*) isr0,  INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(1,  (void*) isr1,  INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(2,  (void*) isr2,  INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(3,  (void*) isr3,  INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(4,  (void*) isr4,  INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(5,  (void*) isr5,  INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(6,  (void*) isr6,  INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(7,  (void*) isr7,  INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(8,  (void*) isr8,  INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(9,  (void*) isr9,  INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(10, (void*) isr10, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(11, (void*) isr11, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(12, (void*) isr12, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(13, (void*) isr13, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(14, (void*) isr14, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(15, (void*) isr15, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(16, (void*) isr16, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(17, (void*) isr17, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(18, (void*) isr18, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(19, (void*) isr19, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(20, (void*) isr20, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(21, (void*) isr21, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(22, (void*) isr22, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(23, (void*) isr23, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(24, (void*) isr24, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(25, (void*) isr25, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(26, (void*) isr26, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(27, (void*) isr27, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(28, (void*) isr28, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(29, (void*) isr29, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(30, (void*) isr30, INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(31, (void*) isr31, INTERRUPT_GATE_FLAGS);

    kRemapPIC();

    kSetIsrHandler(IRQ0,  (void*)kIrq0,   INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(IRQ1,  (void*)kIrq1,   INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(IRQ2,  (void*)kIrq2,   INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(IRQ3,  (void*)kIrq3,   INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(IRQ4,  (void*)kIrq4,   INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(IRQ5,  (void*)kIrq5,   INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(IRQ6,  (void*)kIrq6,   INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(IRQ7,  (void*)kIrq7,   INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(IRQ8,  (void*)kIrq8,   INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(IRQ9,  (void*)kIrq9,   INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(IRQ10, (void*)kIrq10,  INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(IRQ11, (void*)kIrq11,  INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(IRQ12, (void*)kIrq12,  INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(IRQ13, (void*)kIrq13,  INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(IRQ14, (void*)kIrq14,  INTERRUPT_GATE_FLAGS);
    kSetIsrHandler(IRQ15, (void*)kIrq15,  INTERRUPT_GATE_FLAGS);

    asm volatile ("lidt %0" : : "memory"(g_idtr));
    asm volatile ("sti");
}

void kRemapPIC()
{
    kPortByteOut(MASTER_PIC_COMMAND, ICW1_INIT | ICW1_ICW4); // write ICW1 to PICM, we are gonna write commands to PICM
    kPortByteOut(SLAVE_PIC_COMMAND,  ICW1_INIT | ICW1_ICW4); // write ICW1 to PICS, we are gonna write commands to PICS

    kPortByteOut(MASTER_PIC_DATA, 0x20);    // remap Master PIC to 0x20 (32 decimal - beginning of IRQ)
    kPortByteOut(SLAVE_PIC_DATA,  0x28);    // remap Slave PIC to 0x28 (40 decimal)

    kPortByteOut(MASTER_PIC_DATA, 0x04);    // IRQ2 -> connection to slave
    kPortByteOut(SLAVE_PIC_DATA,  0x02);

    kPortByteOut(MASTER_PIC_DATA, ICW1_ICW4);    // write ICW4 to PICM, we are gonna write commands to PICM
    kPortByteOut(SLAVE_PIC_DATA,  ICW1_ICW4);    // write ICW4 to PICS, we are gonna write commands to PICS

    kPortByteOut(MASTER_PIC_DATA, 0x0);     // enable all IRQs on PICM
    kPortByteOut(SLAVE_PIC_DATA,  0x0);     // enable all IRQs on PICS
}

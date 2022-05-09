#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TRAP_GATE_FLAGS             0x8F    // P = 1, DPL = 00, S = 0, Type = 1111 (32bit trap gate)
#define INTERRUPT_GATE_FLAGS        0x8E    // P = 1, DPL = 00, S = 0, Type = 1110 (32bit interrupt gate)
#define INTERRUPT_GATE_USER_FLAGS   0xEE    // P = 1, DPL = 11, S = 0, Type = 1110 (32bit interrupt gate called from privilege level 3)

// Interrupt error code type
typedef unsigned long int interrupt_error_code_t;

typedef struct {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t	    ist;          // The IST in the TSS that the CPU will load into RSP; set to zero for now
	uint8_t     flags;        // Type and attributes; see the IDT page
	uint16_t    isr_mid;      // The higher 16 bits of the lower 32 bits of the ISR's address
	uint32_t    isr_high;     // The higher 32 bits of the ISR's address
	uint32_t    reserved;     // Set to zero
} __attribute__((packed)) IdtEntry_t;

typedef struct {
	uint16_t	size;
	uint64_t	address;
} __attribute__((packed)) Idtr_t;

__attribute__((aligned(0x10)))
extern IdtEntry_t g_idt[256]; // Create an array of IDT entries; aligned for performance

extern Idtr_t g_idtr;

typedef struct {
    uint64_t ds;
    uint64_t rdi, rsi, rbp, rsp, rbx, rdx, rcx, rax;
    uint64_t int_no, err_no;
    uint64_t rip, cs, eflags, usersp, ss;
} __attribute__((packed)) InterruptFrame_t;

typedef struct {
    uint64_t rip, cs, eflags, rsp, ss;
} __attribute__((packed)) NakedInterruptFrame_t;

void kKernelIsrHandler(InterruptFrame_t frame);
void kKernelIrqHandler(NakedInterruptFrame_t* frame, uint8_t int_no);

typedef void(*UserIrqHandler_t)(NakedInterruptFrame_t*, uint8_t int_no);
void kRegisterUserIrqHandler(uint8_t entry, UserIrqHandler_t handler);

void kSetIsrHandler(uint8_t isr_index, void* isr, uint8_t flags);

void kInitializeIDT();

/* ISRs reserved for CPU exceptions */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

__attribute__((interrupt)) void kIrq0(NakedInterruptFrame_t* frame);
__attribute__((interrupt)) void kIrq1(NakedInterruptFrame_t* frame);
__attribute__((interrupt)) void kIrq2(NakedInterruptFrame_t* frame);
__attribute__((interrupt)) void kIrq3(NakedInterruptFrame_t* frame);
__attribute__((interrupt)) void kIrq4(NakedInterruptFrame_t* frame);
__attribute__((interrupt)) void kIrq5(NakedInterruptFrame_t* frame);
__attribute__((interrupt)) void kIrq6(NakedInterruptFrame_t* frame);
__attribute__((interrupt)) void kIrq7(NakedInterruptFrame_t* frame);
__attribute__((interrupt)) void kIrq8(NakedInterruptFrame_t* frame);
__attribute__((interrupt)) void kIrq9(NakedInterruptFrame_t* frame);
__attribute__((interrupt)) void kIrq10(NakedInterruptFrame_t* frame);
__attribute__((interrupt)) void kIrq11(NakedInterruptFrame_t* frame);
__attribute__((interrupt)) void kIrq12(NakedInterruptFrame_t* frame);
__attribute__((interrupt)) void kIrq13(NakedInterruptFrame_t* frame);
__attribute__((interrupt)) void kIrq14(NakedInterruptFrame_t* frame);
__attribute__((interrupt)) void kIrq15(NakedInterruptFrame_t* frame);

#define IRQ0  32
#define IRQ1  33
#define IRQ2  34
#define IRQ3  35
#define IRQ4  36
#define IRQ5  37
#define IRQ6  38
#define IRQ7  39
#define IRQ8  40
#define IRQ9  41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

// PIC Chip Control
#define MASTER_PIC_COMMAND  0x20
#define MASTER_PIC_DATA     0x21

#define SLAVE_PIC_COMMAND   0xA0
#define SLAVE_PIC_DATA      0xA1

// Initialization Control Word
#define ICW1_INIT           0x10
#define ICW1_ICW4           0x01
#define ICW4_8086           0x01

void kRemapPIC();

#ifdef __cplusplus
} // extern "C"
#endif

#endif

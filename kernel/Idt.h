#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include "typedefs.h"

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
static IdtEntry_t g_idt[256]; // Create an array of IDT entries; aligned for performance

static Idtr_t g_idtr;

typedef struct {
    uint64_t ds;
    uint64_t rdi, rsi, rbp, rsp, rbx, rdx, rcx, rax;
    uint64_t int_no, err_no;
    uint64_t rip, cs, eflags, usersp, ss;
} __attribute__((packed)) InterruptFrame_t;

void kKernelIsrHandler(InterruptFrame_t frame);

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

#endif

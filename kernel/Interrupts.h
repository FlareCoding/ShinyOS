#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include "typedefs.h"

#define TRAP_GATE_FLAGS     0x8F    // P = 1, DPL = 00, S = 0, Type = 1111 (32bit trap gate)
#define INT_GATE_FLAGS      0x8E    // P = 1, DPL = 00, S = 0, Type = 1110 (32bit interrupt gate)
#define INT_GATE_USER_FLAGS 0xEE    // P = 1, DPL = 11, S = 0, Type = 1110 (32bit interrupt gate called from privilege level 3)

// Interrupt error code type
typedef unsigned long int interrupt_error_code_t;

typedef struct {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t	    ist;          // The IST in the TSS that the CPU will load into RSP; set to zero for now
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isr_mid;      // The higher 16 bits of the lower 32 bits of the ISR's address
	uint32_t    isr_high;     // The higher 32 bits of the ISR's address
	uint32_t    reserved;     // Set to zero
} __attribute__((packed)) IdtEntry_t;

typedef struct {
	uint16_t	size;
	uint64_t	address;
} __attribute__((packed)) Idtr_t;

__attribute__((aligned(0x10))) 
extern IdtEntry_t g_IDT[256]; // Create an array of IDT entries; aligned for performance

extern Idtr_t g_idtr;

typedef struct {
    uint32_t eip;       // Instruction pointer
    uint32_t cs;        // Code segment
    uint32_t eflags;    // Flags
    uint32_t esp;       // Stack pointer
    uint32_t ss;        // Special segment
} __attribute__((packed)) InterruptFrame_t;

// Sets up the interrupt descriptor table
void kInitializeIDT();

// Specific interrupt service routines
__attribute__((interrupt))
void kKeyboardISR(InterruptFrame_t* interrupt_frame);

// Universal IRQ handler
void kInterruptRequestHandler(InterruptFrame_t* frame, uint32_t intno);

// CPU Exception Service Routines
__attribute__((interrupt)) void kISR0(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR1(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR2(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR3(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR4(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR5(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR6(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR7(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR8(InterruptFrame_t* frame, interrupt_error_code_t errno);
__attribute__((interrupt)) void kISR9(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR10(InterruptFrame_t* frame, interrupt_error_code_t errno);
__attribute__((interrupt)) void kISR11(InterruptFrame_t* frame, interrupt_error_code_t errno);
__attribute__((interrupt)) void kISR12(InterruptFrame_t* frame, interrupt_error_code_t errno);
__attribute__((interrupt)) void kISR13(InterruptFrame_t* frame, interrupt_error_code_t errno);
__attribute__((interrupt)) void kISR14(InterruptFrame_t* frame, interrupt_error_code_t errno);
__attribute__((interrupt)) void kISR15(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR16(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR17(InterruptFrame_t* frame, interrupt_error_code_t errno);
__attribute__((interrupt)) void kISR18(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR19(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR20(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR21(InterruptFrame_t* frame, interrupt_error_code_t errno);
__attribute__((interrupt)) void kISR22(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR23(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR24(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR25(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR26(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR27(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR28(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR29(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR30(InterruptFrame_t* frame);
__attribute__((interrupt)) void kISR31(InterruptFrame_t* frame);

// Hardware Interrupts
__attribute__((interrupt)) void kIRQ0(InterruptFrame_t* frame);
__attribute__((interrupt)) void kIRQ1(InterruptFrame_t* frame);
__attribute__((interrupt)) void kIRQ2(InterruptFrame_t* frame);
__attribute__((interrupt)) void kIRQ3(InterruptFrame_t* frame);
__attribute__((interrupt)) void kIRQ4(InterruptFrame_t* frame);
__attribute__((interrupt)) void kIRQ5(InterruptFrame_t* frame);
__attribute__((interrupt)) void kIRQ6(InterruptFrame_t* frame);
__attribute__((interrupt)) void kIRQ7(InterruptFrame_t* frame);
__attribute__((interrupt)) void kIRQ8(InterruptFrame_t* frame);
__attribute__((interrupt)) void kIRQ9(InterruptFrame_t* frame);
__attribute__((interrupt)) void kIRQ10(InterruptFrame_t* frame);
__attribute__((interrupt)) void kIRQ11(InterruptFrame_t* frame);
__attribute__((interrupt)) void kIRQ12(InterruptFrame_t* frame);
__attribute__((interrupt)) void kIRQ13(InterruptFrame_t* frame);
__attribute__((interrupt)) void kIRQ14(InterruptFrame_t* frame);
__attribute__((interrupt)) void kIRQ15(InterruptFrame_t* frame);

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

#endif

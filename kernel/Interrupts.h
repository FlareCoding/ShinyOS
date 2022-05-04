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

// Default exception handler WITHOUT the error code
__attribute__((interrupt))
void kDefaultExceptionHandler(InterruptFrame_t* interrupt_frame);

// Default exception handler WITH the error code
__attribute__((interrupt))
void kDefaultExceptionHandlerErrCode(InterruptFrame_t* interrupt_frame, interrupt_error_code_t error_code);

// Default interrupt handler
__attribute__((interrupt))
void kDefaultInterruptHandler(InterruptFrame_t* interrupt_frame);

// Adds an interrupt service routine to the IDT
void kSetInterruptServiceRoutine(uint8_t isr_index, void* isr, uint8_t flags);

// Sets up the interrupt descriptor table
void kInitializeIDT();

__attribute__ ((interrupt))
void div_by_0_handler(InterruptFrame_t* frame);

#endif

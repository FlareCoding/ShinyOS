#include "Interrupts.h"
#include <drivers/VGAScreenDriver.h>


void kDefaultExceptionHandler(InterruptFrame_t* interrupt_frame)
{
    kPrint("kDefaultExceptionHandler called\n");
}

void kDefaultExceptionHandlerErrCode(InterruptFrame_t* interrupt_frame, interrupt_error_code_t error_code)
{
    kPrint("kDefaultExceptionHandlerErrCode called\n");
}

void kDefaultInterruptHandler(InterruptFrame_t* interrupt_frame)
{
    kPrint("kDefaultInterruptHandler called\n");
}

void kSetInterruptServiceRoutine(uint8_t isr_index, void* isr, uint8_t flags)
{
    IdtEntry_t* descriptor = &g_IDT[isr_index];

    descriptor->isr_low     = (uint16_t)((uint64_t)isr & 0x000000000000ffff);
    descriptor->kernel_cs   = 0x8;
    descriptor->ist         = 0x0;
    descriptor->attributes  = flags; // type and attributes
    descriptor->isr_mid     = (uint16_t)(((uint64_t)isr & 0x00000000ffff0000) >> 16);
    descriptor->isr_high    = (uint32_t)(((uint64_t)isr & 0xffffffff00000000) >> 32);
    descriptor->reserved    = 0;
}

void kInitializeIDT()
{
    g_idtr.size    = (uint16_t)(sizeof(IdtEntry_t) * 256);
    g_idtr.address = (uint64_t)&g_IDT[0];

    // Setup default exception handlers (ISRs 0-31)
    for (uint8_t entry = 0; entry < 32; entry++)
    {
        switch (entry)
        {
        case 8:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 17:
        case 21:
        {
            kSetInterruptServiceRoutine(entry, kDefaultExceptionHandlerErrCode, TRAP_GATE_FLAGS);
            break;
        }
        default:
        {
            kSetInterruptServiceRoutine(entry, kDefaultExceptionHandler, TRAP_GATE_FLAGS);
            break;
        }
        }
    }

    // Setup default interrupt handlers (ISRs 32-255)
    for (uint16_t entry = 32; entry < 256; entry++)
    {
        kSetInterruptServiceRoutine(entry, kDefaultInterruptHandler, INT_GATE_FLAGS);
    }

    __asm__ __volatile__ ("lidt %0" : : "memory"(g_idtr));
}

void div_by_0_handler(InterruptFrame_t* frame)
{
    kPrint("Divide by 0 error - exception handled");

    frame->eip++;
}

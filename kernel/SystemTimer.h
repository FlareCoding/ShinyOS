#ifndef SYSTEM_TIMER_H
#define SYSTEM_TIMER_H
#include "Idt.h"

// Called by the interrupt service routine
void kHandleSystemClockInterrupt(NakedInterruptFrame_t* frame, uint8_t int_no);

// Returns the number of system clock ticks since boot time
uint64_t kGetSystemClockTime();

#endif

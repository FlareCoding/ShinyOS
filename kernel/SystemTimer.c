#include "SystemTimer.h"
#include <drivers/VGAScreenDriver.h>

uint64_t g_SystemClockTime = 0;

void kHandleSystemClockInterrupt(NakedInterruptFrame_t* frame, uint8_t int_no)
{
    ++g_SystemClockTime;
}

uint64_t kGetSystemClockTime()
{
    return g_SystemClockTime;
}

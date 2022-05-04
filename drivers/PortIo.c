#include "PortIo.h"

uint8_t kPortByteIn(uint16_t port)
{
    uint8_t result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void kPortByteOut(uint16_t port, uint8_t data)
{
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint16_t kPortWordIn(uint16_t port)
{
    uint16_t result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void kPortWordOut(uint16_t port, uint16_t data)
{
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

void kRemapPIC()
{
    // For storing the mask of the PIC chip.
    uint8_t a1, a2;

    a1 = kPortByteIn(PIC1_DATA);
    a2 = kPortByteIn(PIC2_DATA);

    // Tells the master PIC chip to start the initialization sequence.
    kPortByteOut(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);

    // Tells the slave PIC chip to start the initialization sequence.
    kPortByteOut(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    // Specify offsets
    kPortByteOut(PIC1_DATA, 0);
    kPortByteOut(PIC2_DATA, 8);
    kPortByteOut(PIC1_DATA, 4);
    kPortByteOut(PIC2_DATA, 2);
    kPortByteOut(PIC1_DATA, ICW4_8086);
    kPortByteOut(PIC2_DATA, ICW4_8086);

    // Restored saved masks.
    kPortByteOut(PIC1_DATA, a1);
    kPortByteOut(PIC2_DATA, a2);
}

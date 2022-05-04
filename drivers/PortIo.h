#ifndef PORTS_H
#define PORTS_H

#include <kernel/typedefs.h>

uint8_t         kPortByteIn(uint16_t port);
void            kPortByteOut(uint16_t port, uint8_t data);
uint16_t        kPortWordIn(uint16_t port);
void            kPortWordOut(uint16_t port, uint16_t data);

// PIC Chip Control
#define PIC1_COMMAND    0x20
#define PIC1_DATA       0x21

#define PIC2_COMMAND    0xA0
#define PIC2_DATA       0xA1

// Initialization Control Word
#define ICW1_INIT       0x10
#define ICW1_ICW4       0x01
#define ICW4_8086       0x01

void            kRemapPIC();

#endif

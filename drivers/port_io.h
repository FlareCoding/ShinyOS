#ifndef PORTS_H
#define PORTS_H

#include <kernel/typedefs.h>

uint8_t         kPortByteIn(uint16_t port);
void            kPortByteOut(uint16_t port, uint8_t data);
uint16_t        kPortWordIn(uint16_t port);
void            kPortWordOut(uint16_t port, uint16_t data);

#endif

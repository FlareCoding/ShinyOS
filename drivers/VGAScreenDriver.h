#ifndef VGA_SCREEN_DRIVER_H
#define VGA_SCREEN_DRIVER_H
#include <kernel/KernelMemory.h>
#include "PortIo.h"

#define VGA_MEMORY_ADDRESS              0xb8000
#define VGA_SCREEN_MAX_ROWS             25
#define VGA_SCREEN_MAX_COLS             80
#define VGA_SCREEN_MAX_CURSOR_INDEX     2000
#define VGA_COLOR_WHITE_ON_BLACK        0x0F
#define VGA_COLOR_RED_ON_WHITE          0xF4

#define IO_PORT_VGA_SCREEN_CTRL         0x3D4
#define IO_PORT_VGA_SCREEN_DATA         0x3D5

#define VGA_FOREGROUND_BLACK            0x00
#define VGA_FOREGROUND_BLUE             0x01
#define VGA_FOREGROUND_GREEN            0x02
#define VGA_FOREGROUND_CYAN             0x03
#define VGA_FOREGROUND_RED              0x04
#define VGA_FOREGROUND_MAGENTA          0x05
#define VGA_FOREGROUND_BROWN            0x06
#define VGA_FOREGROUND_LIGHTGRAY        0x07
#define VGA_FOREGROUND_DARKGRAY         0x08
#define VGA_FOREGROUND_LIGHTBLUE        0x09
#define VGA_FOREGROUND_LIGHTGREEN       0x0A
#define VGA_FOREGROUND_LIGHTCYAN        0x0B
#define VGA_FOREGROUND_LIGHTRED         0x0C
#define VGA_FOREGROUND_LIGHTMAGENTA     0x0D
#define VGA_FOREGROUND_YELLOW           0x0E
#define VGA_FOREGROUND_WHITE            0x0F

#define VGA_BACKGROUND_BLACK            0x00
#define VGA_BACKGROUND_BLUE             0x10
#define VGA_BACKGROUND_GREEN            0x20
#define VGA_BACKGROUND_CYAN             0x30
#define VGA_BACKGROUND_RED              0x40
#define VGA_BACKGROUND_MAGENTA          0x50
#define VGA_BACKGROUND_BROWN            0x60
#define VGA_BACKGROUND_LIGHTGRAY        0x70
#define VGA_BACKGROUND_BLINKING_BLACK   0x80
#define VGA_BACKGROUND_BLINKING_BLUE    0x90
#define VGA_BACKGROUND_BLINKING_GREEN   0xA0
#define VGA_BACKGROUND_BLINKING_CYAN    0xB0
#define VGA_BACKGROUND_BLINKING_RED     0xC0
#define VGA_BACKGROUND_BLINKING_MAGENTA 0xD0
#define VGA_BACKGROUND_BLINKING_YELLOW  0xE0
#define VGA_BACKGROUND_BLINKING_WHITE   0xF0

#define VGA_DEFAULT_COLOR (VGA_BACKGROUND_BLACK | VGA_FOREGROUND_CYAN)

int32_t     kGetVGACursorOffset();
void        kSetVGACursorOffset(int32_t offset);

int32_t     kGetVGACursorOffsetFromColRow(int32_t col, int32_t row);
int32_t     kGetVGACursorRowFromOffset(int32_t offset);
int32_t     kGetVGACursorColFromOffset(int32_t offset);

void        kClearVGAScreenBuffer();

int32_t     kPrintChar(char c, int32_t col, int32_t row, int8_t attribs);

void        kPrintColRow(const char* buffer, int32_t col, int32_t row);
void        kPrint(const char* buffer);

#endif

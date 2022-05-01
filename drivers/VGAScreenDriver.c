#include "VGAScreenDriver.h"

int32_t kGetVGACursorOffset()
{
    kPortByteOut(IO_PORT_VGA_SCREEN_CTRL, 14);
    int32_t offset = kPortByteIn(IO_PORT_VGA_SCREEN_DATA) << 8;

    kPortByteOut(IO_PORT_VGA_SCREEN_CTRL, 15);
    offset += kPortByteIn(IO_PORT_VGA_SCREEN_DATA);

    return offset * 2;
}

void kSetVGACursorOffset(int32_t offset)
{
    offset /= 2;
    kPortByteOut(IO_PORT_VGA_SCREEN_CTRL, 14);
    kPortByteOut(IO_PORT_VGA_SCREEN_DATA, (uint8_t)(offset >> 8));
    kPortByteOut(IO_PORT_VGA_SCREEN_CTRL, 15);
    kPortByteOut(IO_PORT_VGA_SCREEN_DATA, (uint8_t)(offset & 0xff));
}

int32_t kGetVGACursorOffsetFromColRow(int32_t col, int32_t row)
{
    return 2 * (row * VGA_SCREEN_MAX_COLS + col);
}

int32_t kGetVGACursorRowFromOffset(int32_t offset)
{
    return offset / (2 * VGA_SCREEN_MAX_COLS);
}

int32_t kGetVGACursorColFromOffset(int32_t offset)
{
    return (offset - (kGetVGACursorRowFromOffset(offset) * 2 * VGA_SCREEN_MAX_COLS)) / 2;
}

void kClearVGAScreenBuffer()
{
    int32_t screen_size = VGA_SCREEN_MAX_COLS * VGA_SCREEN_MAX_ROWS;
    char* vga_memory = (char*)VGA_MEMORY_ADDRESS;

    int32_t i;
    for (i = 0; i < screen_size; ++i) {
        vga_memory[i * 2] = ' ';
        vga_memory[i * 2 + 1] = VGA_COLOR_WHITE_ON_BLACK;
    }

    kSetVGACursorOffset(kGetVGACursorOffsetFromColRow(0, 0));
}

int32_t kPrintChar(char c, int32_t col, int32_t row, int8_t attribs)
{
    // Get the vga memory address
    int8_t* vga_memory = (int8_t*)VGA_MEMORY_ADDRESS;

    // Color attribs
    if (!attribs)
        attribs = VGA_COLOR_WHITE_ON_BLACK;

    // Cursor offset/position
    int32_t offset = 0;

    if (col >= 0 && row >= 0)
        offset = kGetVGACursorOffsetFromColRow(col, row);
    else
        offset = kGetVGACursorOffset();

    // Writing the character into memory
    switch (c)
    {
    case '\n':
    {
        row = kGetVGACursorRowFromOffset(offset);
        offset = kGetVGACursorOffsetFromColRow(0, row + 1);
        break;
    }
    default:
    {
        vga_memory[offset] = c;
        vga_memory[offset + 1] = attribs;
        offset += 2;
        break;
    }
    }

    // Update the cursor offset
    kSetVGACursorOffset(offset);

    return offset;
}

void kPrintColRow(const char* buffer, int32_t col, int32_t row)
{
    int32_t offset = 0;

    if (col >= 0 && row >= 0)
    {
        offset = kGetVGACursorOffsetFromColRow(col, row);
    }
    else
    {
        offset = kGetVGACursorOffset();
        row = kGetVGACursorRowFromOffset(offset);
        col = kGetVGACursorColFromOffset(offset);
    }

    int32_t i = 0;
    while (buffer[i] != 0)
    {
        offset = kPrintChar(buffer[i++], col, row, VGA_COLOR_WHITE_ON_BLACK);
        row = kGetVGACursorRowFromOffset(offset);
        col = kGetVGACursorColFromOffset(offset);
    }
}

void kPrint(const char* buffer)
{
    kPrintColRow(buffer, -1, -1);
}

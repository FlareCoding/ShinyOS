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

    // Checking if text needs to be scrolled up
    if (offset >= VGA_SCREEN_MAX_CURSOR_INDEX * 2) {
        uint8_t*    copy_start_addr = (uint8_t*)VGA_MEMORY_ADDRESS + (VGA_SCREEN_MAX_COLS * 2);
        size_t      copy_size = (VGA_SCREEN_MAX_CURSOR_INDEX - VGA_SCREEN_MAX_COLS) * 2;

        // Copy all the lines one line above
        kCopyMemory((void*)VGA_MEMORY_ADDRESS, copy_start_addr, copy_size);

        // Empty out the last line
        uint8_t* last_line = (uint8_t*)VGA_MEMORY_ADDRESS + kGetVGACursorOffsetFromColRow(0, VGA_SCREEN_MAX_ROWS - 1);
        for (size_t i = 0; i < VGA_SCREEN_MAX_COLS * 2; i += 2)
        {
            last_line[i] = ' ';
            last_line[i + 1] = VGA_COLOR_WHITE_ON_BLACK;
        }

        offset -= 2 * VGA_SCREEN_MAX_COLS;
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

void kPrintHex(uint64_t val)
{
    char hex_string_buffer[128];

    uint64_t* val_ptr = &val;
    uint8_t size = sizeof(uint64_t) * 2 - 1;

    for (uint8_t i = 0; i < size; i++)
    {
        uint8_t* ptr = ((uint8_t*)val_ptr + i);
        uint8_t tmp = ((*ptr & 0xF0) >> 4);
        hex_string_buffer[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : 48);
        tmp = ((*ptr & 0x0F));
        hex_string_buffer[size - (i * 2)] = tmp + (tmp > 9 ? 55 : 48);
    }

    hex_string_buffer[size + 1] = '\0';

    kPrint("0x");
    kPrint(hex_string_buffer);
}

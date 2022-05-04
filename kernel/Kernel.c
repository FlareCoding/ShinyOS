#include <drivers/VGAScreenDriver.h>
#include <kernel/Interrupts.h>

void _kmain() {
    // Initialize IDT and setup interrupt service routines
    kInitializeIDT();

    //kSetInterruptServiceRoutine(0, div_by_0_handler, TRAP_GATE_FLAGS);

    kClearVGAScreenBuffer();
    kPrintColRow("X", 1, 6);
    kPrintColRow("This text spans multiple lines", 75, 10);
    kPrintColRow("There is a line\nbreak", 0, 20);
    kPrint("There is a line\nbreak");
    kPrintColRow("What happens when we run out of space?", 45, 24);
    kPrint("\nNew Hello World!\nWow\nIt scrolls!\n");
    kPrint("r");
    kPrint("a");
    kPrint("x");
}

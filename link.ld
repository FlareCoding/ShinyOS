OUTPUT_FORMAT(binary)

ENTRY(_kmain)

INPUT
(
    kernel_loader.o

    kernel/Kernel.o
    kernel/KernelMemory.o
    kernel/Interrupts.o

    drivers/PortIo.o
    drivers/VGAScreenDriver.o
)

OUTPUT
(
    kernel.bin
)

SECTIONS
{
    . = 0x8000;

    .text : ALIGN(0x1000)
    {
        *(.text)
    }

    .idt BLOCK(0x1000) : ALIGN(0x1000)
    {
        g_IDT = .;
        g_idtr = .;
        . = . + 0x1000;
    }

    .data : ALIGN(0x1000)
    {
        *(.data)
    }

    .rodata : ALIGN(0x1000)
    {
        *(.rodata)
    }

    .bss : ALIGN(0x1000)
    {
        *(COMMON)
        *(.bss)
    }
}
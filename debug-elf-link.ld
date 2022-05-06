OUTPUT_FORMAT(elf64-x86-64)

ENTRY(_kmain)

INPUT
(
    kernel_loader.o

    kernel/Kernel.o
    kernel/KernelMemory.o
    kernel/Idt.o
    kernel/Interrupts.o

    drivers/PortIo.o
    drivers/VGAScreenDriver.o
    drivers/KeyboardDriver.o
)

OUTPUT
(
    kernel.elf
)

SECTIONS
{
    . = 0x8000;

    .text : ALIGN(0x1000)
    {
        *(.text)
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
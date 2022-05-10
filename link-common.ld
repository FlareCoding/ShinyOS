ENTRY(_kmain)

INPUT
(
    kernel_loader.o

    kernel/Kernel.o
    kernel/KernelMemory.o
    kernel/Idt.o
    kernel/Interrupts.o
    kernel/SystemTimer.o
    kernel/MemoryMap.o
    kernel/MemoryHeap.o
    
    kernel/paging/PageBitmap.o
    kernel/paging/Paging.o
    kernel/paging/PageFrameAllocator.o
    kernel/paging/PagingUtils.o
    kernel/paging/PageTableManager.o

    drivers/PortIo.o
    drivers/VGAScreenDriver.o
    drivers/KeyboardDriver.o
)

SECTIONS
{
    . = 0x8000;

    _KernelAddressStart = .;

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

    _KernelAddressEnd = .;
}
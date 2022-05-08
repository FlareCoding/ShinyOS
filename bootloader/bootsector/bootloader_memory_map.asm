[bits 16]

MEMORY_MAP_ADDRESS equ 0x5000

_BootloaderLoadMemoryMap:
    mov di, MEMORY_MAP_ADDRESS  ; Where the memory map table will be loaded to
    mov edx, 0x534D4150         ; In ASCII stands for SMAP
    xor ebx, ebx

.loop:
    mov eax, 0xE820
    mov ecx, 24
    int 0x15

    cmp ebx, 0
    je .done

    add di, 24
    jmp .loop

.done:
    ret

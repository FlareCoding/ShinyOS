; Protected Mode <=> PM
[bits 32]

_PMInit:
    mov ax, DATA_SEG ; Update the segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000 ; Move the base and stack pointers to the top of the free space
    mov esp, ebp

    jmp _PMStart

_PMStart:
    mov esi, PROTECTED_MODE_ENTERED_MSG
    call _PMPrintString

    jmp $

PROTECTED_MODE_ENTERED_MSG db 'Entered protected mode (32bits)', 0



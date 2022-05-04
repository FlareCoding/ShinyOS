; Protected Mode <=> PM
[bits 32]

_PMInit:
    mov ax, DATA_SEG ; Update the segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x9000 ; Move the base and stack pointers to the top of the free space
    mov esp, ebp

    jmp _PMStart

_PMStart:
    mov esi, PROTECTED_MODE_ENTERED_MSG
    call _PMPrintString

    ; Detect CPUID instruction and check
    ; if long mode is available.
    call _PMDetectCpuid
    call _PMDetectLongMode

    ; Set up paging
    call _PMSetupPaging

    ; Fix the GDT to contain 64bit flags
    call _PMEditGDT

    ; Load the 64bit GDT pointer
    lgdt [GDT_DESCRIPTOR]

    ; Enter long mode
    jmp CODE_SEG:_LMStart

PROTECTED_MODE_ENTERED_MSG db 'Entered protected mode (32bits)', 0

%include "bootloader/bootsector/bootloader_print_utils.asm"
%include "bootloader/bootsector/bootloader_gdt.asm"
%include "bootloader/sector2/cpuid.asm"
%include "bootloader/sector2/paging.asm"

[bits 64]
[extern _kmain]

_LMStart:
    mov edi, 0xb8000
    mov rax, 0x0F200F200F200F20
    mov ecx, 500
    rep stosq

    call _kmain
    jmp $

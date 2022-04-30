[bits 16]
[org 0x7c00]

BOOTSECTOR_STACK_BOTTOM equ 0x8000
SECTORS_TO_READ         equ 1
KERNEL_LOADER_OFFSET    equ 0x8000

; Save the boot drive provided by the BIOS
mov [BOOT_DRIVE], dl

; Setup the stack
mov bp, BOOTSECTOR_STACK_BOTTOM
mov sp, bp

; Read the next sectors of the disk
mov bx, KERNEL_LOADER_OFFSET    ; where to place the contents
mov dh, SECTORS_TO_READ         ; sectors to read
mov dl, [BOOT_DRIVE]            ; boot disk number
call _BootsectorLoadDisk

; Switch to 32bit Protected Mode
call _BootloaderSwitchToProtectedMode

jmp $

_BootloaderSwitchToProtectedMode:
    call _BootloaderEnableA20Line   ; enable A20 address line
    cli                             ; disable interrupts
    lgdt [GDT_DESCRIPTOR]           ; load the GDT descriptor

    mov eax, cr0
    or eax, 0x1                     ; set 32-bit mode bit in cr0
    mov cr0, eax

    jmp CODE_SEG:KERNEL_LOADER_OFFSET

_BootloaderEnableA20Line:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

BOOT_DRIVE db 0

%include "bootsector/bootsector_print_utils.asm"
%include "bootsector/bootsector_disk_utils.asm"
%include "bootsector/bootsector_gdt.asm"

; Fill with 510 zeros minus the size of the previous code
times 510-($-$$) db 0

; Bootloader signature
dw 0xAA55

[bits 16]
[org 0x7c00]

BOOTSECTOR_STACK_BOTTOM equ 0x8000
SECTORS_TO_READ         equ 1
KERNEL_OFFSET           equ 0x1000

; Save the boot drive provided by the BIOS
mov [BOOT_DRIVE], dl

; Setup the stack
mov bp, BOOTSECTOR_STACK_BOTTOM
mov sp, bp

; Read the next sectors of the disk
mov bx, KERNEL_OFFSET       ; where to place the contents
mov dh, SECTORS_TO_READ     ; sectors to read
mov dl, [BOOT_DRIVE]        ; boot disk number
call _BootsectorLoadDisk

; Print log message 'bootsector started'
call _BootsectorPrintChecksum

; Switch to 32bit Protected Mode
call _BootloaderSwitchToProtectedMode

jmp $

_BootloaderSwitchToProtectedMode:
    cli                     ; disable interrupts
    lgdt [gdt_descriptor]   ; load the GDT descriptor

    mov eax, cr0
    or eax, 0x1             ; set 32-bit mode bit in cr0
    mov cr0, eax

    jmp CODE_SEG:_PMInit

; Utility helper functions
_BootsectorPrintChecksum:
    mov si, BOOTSECTOR_CHECKSUM_MSG
    call _BIOSPrintString

    mov dx, [BOOTSECTOR_CHECKSUM]
    call _BIOSPrintHex

    call _BIOSPrintNewLine

    ret

; Log messages
BOOTSECTOR_CHECKSUM_MSG     db 'Bootsector checksum   : ', 0
BOOTSECTOR_CHECKSUM         dw 0x4554

BOOT_DRIVE db 0

%include "bootsector/bootsector_print_utils.asm"
%include "bootsector/bootsector_disk_utils.asm"
%include "bootsector/bootsector_gdt.asm"
%include "bootsector/bootsector_protected_mode.asm"

; Fill with 510 zeros minus the size of the previous code
times 510-($-$$) db 0

; Bootloader signature
dw 0xAA55

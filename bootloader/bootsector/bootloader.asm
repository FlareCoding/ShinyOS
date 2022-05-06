[org 0x7c00]
[bits 16]

; Offsets
BOOTSECTOR_STACK_BOTTOM equ 0x8000
SECTORS_TO_READ         equ 30
KERNEL_LOADER_OFFSET    equ 0x8000

section .text
_BootloaderBPB:
    jmp _BootloaderMain
    times 3 - ($ - $$) db 0x90   ; Support 2 or 3 byte encoded JMPs before BPB.

    ; BPB (Bios Parameter Block)
    OEMname:           db    "mkfs.fat"  ; mkfs.fat is what OEMname mkdosfs uses
    bytesPerSector:    dw    512
    sectPerCluster:    db    1
    reservedSectors:   dw    1
    numFAT:            db    2
    numRootDirEntries: dw    224
    numSectors:        dw    2880
    mediaType:         db    0xf0
    numFATsectors:     dw    9
    sectorsPerTrack:   dw    18
    numHeads:          dw    2
    numHiddenSectors:  dd    0
    numSectorsHuge:    dd    0
    driveNum:          db    0
    reserved:          db    0
    signature:         db    0x29
    volumeID:          dd    0x6d8f4a2a
    volumeLabel:       db    "SHINYOS    "
    fileSysType:       db    "FAT12   "

_BootloaderMain:
    cli         ; clear interrupts
    cld         ; clear string instructions forward movement

    ; Setup segments and the stack
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov bp, BOOTSECTOR_STACK_BOTTOM
    mov sp, bp

; Save the boot drive index
mov [BOOT_DRIVE], dl

; Read the next sectors of the disk
mov bx, KERNEL_LOADER_OFFSET    ; where to place the contents
mov dh, SECTORS_TO_READ         ; sectors to read
mov dl, [BOOT_DRIVE]            ; boot disk number
call _BootloaderLoadDisk

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

%include "bootloader/bootsector/bootloader_print_utils.asm"
%include "bootloader/bootsector/bootloader_disk_utils.asm"
%include "bootloader/bootsector/bootloader_gdt.asm"

; -------------------------------------------------------------------
; -------------------------------------------------------------------
; Fill the rest with zeroes keeping space for
; the partition table and bootloader signature.
;
; The partition table has four 16 byte entries.
;
times (0x1be - ($ - $$)) db 0

PARTITION_ENTRY_SIZE equ 16

; The following is the first partition table entry (16 bytes)
db 0x80
db 0,2,0
db 0x0f0
db 0x0ff, 0x0ff, 0x0ff
dd 1
dd (10000 * 16 * 63 - 1)

; The remaining partition table entries are zeroed out
times (PARTITION_ENTRY_SIZE * 3) db 0

; Bootloader signature
dw 0xAA55

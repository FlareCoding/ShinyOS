BIOS_DISK_INTERRUPT equ 0x13
DISK_READ_SECTORS equ 0x02

;
; Expected Parameters:
;       dh -> number of sectors to read
;       dl -> drive number
;       bx -> address into where we should load the disk contents
;
; Resources:
;       https://stanislavs.org/helppc/int_13-2.html
;
_BootsectorLoadDisk:
    pusha

    ; Save the provided number of sectors
    ; to read onto the stack.
    push dx

    mov ah, DISK_READ_SECTORS   ; flag for the int13 interrupt
    mov al, dh                  ; number of sectors to read
    mov ch, 0                   ; cylinder = 0
    mov cl, 2                   ; read starting from sector 2
    mov dh, 0                   ; head number = 0
    
    int BIOS_DISK_INTERRUPT     ; call the BIOS interrupt
    jc _BootsectorDiskError     ; the carry flag is set to 1 on error

    pop dx
    cmp al, dh                  ; BIOS also sets 'al' to the # of sectors read. Compare it.
    jne _BootsectorSectorsError

    popa
    ret

_BootsectorDiskError:
    mov si, DISK_ERROR_MSG
    call _BIOSPrintString

    mov dh, ah  ; ah = error code, dl = disk drive that dropped the error
    call _BIOSPrintHex
    call _BIOSPrintNewLine

    jmp $

_BootsectorSectorsError:
    mov si, SECTORS_ERROR_MSG
    call _BIOSPrintString

    mov dh, 0
    mov dl, al
    call _BIOSPrintHex
    call _BIOSPrintNewLine

    jmp $


DISK_ERROR_MSG: db "[-] Failed to read disk, error code: ", 0
SECTORS_ERROR_MSG: db "[-] Incorrect number of sectors read: ", 0

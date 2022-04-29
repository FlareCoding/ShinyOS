[bits 16]

TTY_MODE equ 0x0e
BIOS_PRINT_INTERRUPT equ 0x10

ASCII_NEWLINE equ 0xA
ASCII_CARRIAGE_RETURN equ 0xD

;
; Expected Parameters:
;       al -> character to print
_BIOSPrintChar:
    mov ah, TTY_MODE            ; printing one character at a time in tty mode
    mov bh, 0x0                 ; page number
    mov bl, 0x0A                ; Cyan Foreground on Black Background
    int BIOS_PRINT_INTERRUPT    ; calling the BIOS video interrupt
    ret

;
; Expected Paramters:
;       none
_BIOSPrintNewLine:
    mov al, ASCII_NEWLINE
    call _BIOSPrintChar

    mov al, ASCII_CARRIAGE_RETURN
    call _BIOSPrintChar

    ret

;
; Expected Parameters:
;       si -> pointer to a string
_BIOSPrintString:
.next_char:
    cmp byte [si], 0
    je .done

    mov al, byte [si]
    call _BIOSPrintChar

    inc si
    jmp .next_char

.done:
    ret

;
; Expected Parameters
;       dx -> hex number
_BIOSPrintHex:
    pusha

    mov cx, 0 ; our index variable

; Strategy: get the last char of 'dx', then convert to ASCII
; Numeric ASCII values: '0' (ASCII 0x30) to '9' (0x39), so just add 0x30 to byte N.
; For alphabetic characters A-F: 'A' (ASCII 0x41) to 'F' (0x46) we'll add 0x40
; Then, move the ASCII byte to the correct position on the resulting string
.hex_loop:
    cmp cx, 4 ; loop 4 times
    je .end
    
    ; 1. convert last char of 'dx' to ascii
    mov ax, dx ; we will use 'ax' as our working register
    and ax, 0x000f ; 0x1234 -> 0x0004 by masking first three to zeros
    add al, 0x30 ; add 0x30 to N to convert it to ASCII "N"
    cmp al, 0x39 ; if > 9, add extra 8 to represent 'A' to 'F'
    jle .step2
    add al, 7 ; 'A' is ASCII 65 instead of 58, so 65-58=7

.step2:
    ; 2. get the correct position of the string to place our ASCII char
    ; bx <- base address + string length - index of char
    mov bx, HEX_OUT + 5 ; base + length
    sub bx, cx  ; our index variable
    mov [bx], al ; copy the ASCII char on 'al' to the position pointed by 'bx'
    ror dx, 4 ; 0x1234 -> 0x4123 -> 0x3412 -> 0x2341 -> 0x1234

    ; increment index and loop
    add cx, 1
    jmp .hex_loop

.end:
    mov si, HEX_OUT
    call _BIOSPrintString

    popa
    ret

HEX_OUT:
    db '0x0000', 0 ; reserve memory for our new string


; Protected Mode <=> PM
[bits 32]

VGA_MEMORY_ADDRESS          equ 0xb8000
VGA_COLOR_BLACK_ON_WHITE    equ 0xF

;
; Expected Parameters:
;       esi -> pointer to a string
_PMPrintString:
    pusha
    mov edx, VGA_MEMORY_ADDRESS

.next_char:
    mov al, byte [esi]                  ; get the current character
    mov ah, VGA_COLOR_BLACK_ON_WHITE    ; assign the color

    cmp al, 0           ; check if null terminator was hit
    je .done            ; break out of the loop

    mov word [edx], ax  ; write the character and color into VGA memory
    inc esi             ; move to the next character
    add edx, 2          ; move to the next slot in VGA memory

    jmp .next_char

.done:
    popa
    ret

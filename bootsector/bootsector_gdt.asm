GDT_START:
    ; the GDT starts with a null 8-byte
    dd 0x0 ; 4 bytes
    dd 0x0 ; 4 bytes

; GDT for code segment. base = 0x00000000, length = 0xfffff
; for flags, refer to os-dev.pdf document, page 36
GDT_CODE: 
    dw 0xffff    ; segment length, bits 0-15
    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23
    db 10011010b ; flags (8 bits)
    db 11001111b ; flags (4 bits) + segment length, bits 16-19
    db 0x0       ; segment base, bits 24-31

; GDT for data segment. base and length identical to code segment
GDT_DATA:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

GDT_END:

; GDT descriptor
GDT_DESCRIPTOR:
    dw GDT_END - GDT_START - 1 ; size (16 bit), always one less of its true size
    dd GDT_START ; address (32 bit)

; Constants
CODE_SEG equ GDT_CODE - GDT_START
DATA_SEG equ GDT_DATA - GDT_START

[bits 32]
_PMEditGDT:
    mov [GDT_CODE + 6], byte 10101111b
    mov [GDT_DATA + 6], byte 10101111b
    ret

%macro PUSHALL 0
    push rax
    push rcx
    push rdx
    push rbx
    push rsp
    push rbp
    push rsi
    push rdi
%endmacro

%macro POPALL 0
    pop rdi
    pop rsi
    pop rbp
    pop rsp
    pop rbx
    pop rdx
    pop rcx
    pop rax
%endmacro

[extern kKernelIsrHandler]

; Common ISR handler
kUniversalIsrHandler:
    ; 1. Save CPU state
	PUSHALL             ; Pushes rdi,rsi,rbp,rsp,rbx,rdx,rcx,rax
	mov     ax, ds      ; Lower 16-bits of eax = ds.
	push    rax         ; Save the data segment descriptor
	mov     ax, 0x10    ; Kernel data segment descriptor
	mov     ds, ax
	mov     es, ax
	mov     fs, ax
	mov     gs, ax
	
    ; 2. Call C handler
	call kKernelIsrHandler
	
    ; 3. Restore state
	pop rax 
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	POPALL
	add rsp, 8  ; Cleans up the pushed error code and pushed ISR number
	sti
	iretq       ; Pops 5 things at once: CS, RIP, EFLAGS, SS, and RSP

; Declare global interrupt service routines (CPU exceptions - interrupts 0-31)
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

; 0: Divide By Zero Exception
isr0:
    cli
    push byte 0
    push byte 0
    jmp kUniversalIsrHandler

; 1: Debug Exception
isr1:
    cli
    push byte 0
    push byte 1
    jmp kUniversalIsrHandler

; 2: Non Maskable Interrupt Exception
isr2:
    cli
    push byte 0
    push byte 2
    jmp kUniversalIsrHandler

; 3: Int 3 Exception
isr3:
    cli
    push byte 0
    push byte 3
    jmp kUniversalIsrHandler

; 4: INTO Exception
isr4:
    cli
    push byte 0
    push byte 4
    jmp kUniversalIsrHandler

; 5: Out of Bounds Exception
isr5:
    cli
    push byte 0
    push byte 5
    jmp kUniversalIsrHandler

; 6: Invalid Opcode Exception
isr6:
    cli
    push byte 0
    push byte 6
    jmp kUniversalIsrHandler

; 7: Coprocessor Not Available Exception
isr7:
    cli
    push byte 0
    push byte 7
    jmp kUniversalIsrHandler

; 8: Double Fault Exception (With Error Code!)
isr8:
    cli
    push byte 8
    jmp kUniversalIsrHandler

; 9: Coprocessor Segment Overrun Exception
isr9:
    cli
    push byte 0
    push byte 9
    jmp kUniversalIsrHandler

; 10: Bad TSS Exception (With Error Code!)
isr10:
    cli
    push byte 10
    jmp kUniversalIsrHandler

; 11: Segment Not Present Exception (With Error Code!)
isr11:
    cli
    push byte 11
    jmp kUniversalIsrHandler

; 12: Stack Fault Exception (With Error Code!)
isr12:
    cli
    push byte 12
    jmp kUniversalIsrHandler

; 13: General Protection Fault Exception (With Error Code!)
isr13:
    cli
    push byte 13
    jmp kUniversalIsrHandler

; 14: Page Fault Exception (With Error Code!)
isr14:
    cli
    push byte 14
    jmp kUniversalIsrHandler

; 15: Reserved Exception
isr15:
    cli
    push byte 0
    push byte 15
    jmp kUniversalIsrHandler

; 16: Floating Point Exception
isr16:
    cli
    push byte 0
    push byte 16
    jmp kUniversalIsrHandler

; 17: Alignment Check Exception
isr17:
    cli
    push byte 0
    push byte 17
    jmp kUniversalIsrHandler

; 18: Machine Check Exception
isr18:
    cli
    push byte 0
    push byte 18
    jmp kUniversalIsrHandler

; 19: Reserved
isr19:
    cli
    push byte 0
    push byte 19
    jmp kUniversalIsrHandler

; 20: Reserved
isr20:
    cli
    push byte 0
    push byte 20
    jmp kUniversalIsrHandler

; 21: Reserved
isr21:
    cli
    push byte 0
    push byte 21
    jmp kUniversalIsrHandler

; 22: Reserved
isr22:
    cli
    push byte 0
    push byte 22
    jmp kUniversalIsrHandler

; 23: Reserved
isr23:
    cli
    push byte 0
    push byte 23
    jmp kUniversalIsrHandler

; 24: Reserved
isr24:
    cli
    push byte 0
    push byte 24
    jmp kUniversalIsrHandler

; 25: Reserved
isr25:
    cli
    push byte 0
    push byte 25
    jmp kUniversalIsrHandler

; 26: Reserved
isr26:
    cli
    push byte 0
    push byte 26
    jmp kUniversalIsrHandler

; 27: Reserved
isr27:
    cli
    push byte 0
    push byte 27
    jmp kUniversalIsrHandler

; 28: Reserved
isr28:
    cli
    push byte 0
    push byte 28
    jmp kUniversalIsrHandler

; 29: Reserved
isr29:
    cli
    push byte 0
    push byte 29
    jmp kUniversalIsrHandler

; 30: Reserved
isr30:
    cli
    push byte 0
    push byte 30
    jmp kUniversalIsrHandler

; 31: Reserved
isr31:
    cli
    push byte 0
    push byte 31
    jmp kUniversalIsrHandler

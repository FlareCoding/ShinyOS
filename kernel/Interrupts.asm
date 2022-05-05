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

; Common ISR code
kUniversalIsr:
    ; 1. Save CPU state
	PUSHALL             ; Pushes rdi,rsi,rbp,rsp,rbx,rdx,rcx,rax
	mov     ax, ds      ; Lower 16-bits of eax = ds.
	push    rax         ; save the data segment descriptor
	mov     ax, 0x10    ; kernel data segment descriptor
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
	iret        ; pops 5 things at once: CS, RIP, EFLAGS, SS, and RSP

; Declare global interrupt service routines
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
    jmp kUniversalIsr

; 1: Debug Exception
isr1:
    cli
    push byte 0
    push byte 1
    jmp kUniversalIsr

; 2: Non Maskable Interrupt Exception
isr2:
    cli
    push byte 0
    push byte 2
    jmp kUniversalIsr

; 3: Int 3 Exception
isr3:
    cli
    push byte 0
    push byte 3
    jmp kUniversalIsr

; 4: INTO Exception
isr4:
    cli
    push byte 0
    push byte 4
    jmp kUniversalIsr

; 5: Out of Bounds Exception
isr5:
    cli
    push byte 0
    push byte 5
    jmp kUniversalIsr

; 6: Invalid Opcode Exception
isr6:
    cli
    push byte 0
    push byte 6
    jmp kUniversalIsr

; 7: Coprocessor Not Available Exception
isr7:
    cli
    push byte 0
    push byte 7
    jmp kUniversalIsr

; 8: Double Fault Exception (With Error Code!)
isr8:
    cli
    push byte 8
    jmp kUniversalIsr

; 9: Coprocessor Segment Overrun Exception
isr9:
    cli
    push byte 0
    push byte 9
    jmp kUniversalIsr

; 10: Bad TSS Exception (With Error Code!)
isr10:
    cli
    push byte 10
    jmp kUniversalIsr

; 11: Segment Not Present Exception (With Error Code!)
isr11:
    cli
    push byte 11
    jmp kUniversalIsr

; 12: Stack Fault Exception (With Error Code!)
isr12:
    cli
    push byte 12
    jmp kUniversalIsr

; 13: General Protection Fault Exception (With Error Code!)
isr13:
    cli
    push byte 13
    jmp kUniversalIsr

; 14: Page Fault Exception (With Error Code!)
isr14:
    cli
    push byte 14
    jmp kUniversalIsr

; 15: Reserved Exception
isr15:
    cli
    push byte 0
    push byte 15
    jmp kUniversalIsr

; 16: Floating Point Exception
isr16:
    cli
    push byte 0
    push byte 16
    jmp kUniversalIsr

; 17: Alignment Check Exception
isr17:
    cli
    push byte 0
    push byte 17
    jmp kUniversalIsr

; 18: Machine Check Exception
isr18:
    cli
    push byte 0
    push byte 18
    jmp kUniversalIsr

; 19: Reserved
isr19:
    cli
    push byte 0
    push byte 19
    jmp kUniversalIsr

; 20: Reserved
isr20:
    cli
    push byte 0
    push byte 20
    jmp kUniversalIsr

; 21: Reserved
isr21:
    cli
    push byte 0
    push byte 21
    jmp kUniversalIsr

; 22: Reserved
isr22:
    cli
    push byte 0
    push byte 22
    jmp kUniversalIsr

; 23: Reserved
isr23:
    cli
    push byte 0
    push byte 23
    jmp kUniversalIsr

; 24: Reserved
isr24:
    cli
    push byte 0
    push byte 24
    jmp kUniversalIsr

; 25: Reserved
isr25:
    cli
    push byte 0
    push byte 25
    jmp kUniversalIsr

; 26: Reserved
isr26:
    cli
    push byte 0
    push byte 26
    jmp kUniversalIsr

; 27: Reserved
isr27:
    cli
    push byte 0
    push byte 27
    jmp kUniversalIsr

; 28: Reserved
isr28:
    cli
    push byte 0
    push byte 28
    jmp kUniversalIsr

; 29: Reserved
isr29:
    cli
    push byte 0
    push byte 29
    jmp kUniversalIsr

; 30: Reserved
isr30:
    cli
    push byte 0
    push byte 30
    jmp kUniversalIsr

; 31: Reserved
isr31:
    cli
    push byte 0
    push byte 31
    jmp kUniversalIsr

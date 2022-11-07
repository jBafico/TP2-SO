GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _force_timer_tick
GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler
GLOBAL _int80Handler

GLOBAL _timer_tick_handler

EXTERN irqDispatcher
EXTERN exceptionDispatcher

EXTERN schedule
EXTERN _int80Dispatcher
EXTERN saveRegisters

EXTERN timer_handler


SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro pushAll 0 
    push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
    push fs
    push gs
%endmacro

%macro popAll 0 

    pop gs 
    pop fs
    pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

%macro exceptionHandler 1
	pushState

	mov [GPRv], rax
    mov [GPRv + 1 * 8], rbx
    mov [GPRv + 2 * 8], rcx
    mov [GPRv + 3 * 8], rdx
    mov [GPRv + 4 * 8], rbp
    mov [GPRv + 5 * 8], rsi
    mov [GPRv + 6 * 8], rdi
    mov rax, [rsp+8*18] ;RSP
    mov [GPRv + 7 * 8], rax
    mov [GPRv + 8 * 8], r8
    mov [GPRv + 9 * 8], r9
    mov [GPRv + 10 * 8], r10
    mov [GPRv + 11 * 8], r11
    mov [GPRv + 12 * 8], r12
    mov [GPRv + 13 * 8], r13
    mov [GPRv + 14 * 8], r14
    mov [GPRv + 15 * 8], r15
    mov rax, [rsp+8*15] ;RIP
    mov [GPRv + 16 * 8], rax

	mov rdi, %1 ; pasaje de parametro
	mov rsi, GPRv
	call exceptionDispatcher

	popState
	iretq
%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
    pushState

    mov rdi, 1 ; pasaje de parametro
    call irqDispatcher


    cmp rax, 1
    jne _end
    popState
    pushState

    mov [GPRv], rax
    mov [GPRv + 1 * 8], rbx
    mov [GPRv + 2 * 8], rcx
    mov [GPRv + 3 * 8], rdx
    mov [GPRv + 4 * 8], rbp
    mov [GPRv + 5 * 8], rsi
    mov [GPRv + 6 * 8], rdi
    mov rax, [rsp+8*18]
    mov [GPRv + 7 * 8], rax
    mov [GPRv + 8 * 8], r8
    mov [GPRv + 9 * 8], r9
    mov [GPRv + 10 * 8], r10
    mov [GPRv + 11 * 8], r11
    mov [GPRv + 12 * 8], r12
    mov [GPRv + 13 * 8], r13
    mov [GPRv + 14 * 8], r14
    mov [GPRv + 15 * 8], r15
    mov rax, [rsp+8*15]
    mov [GPRv + 16 * 8], rax

    mov rdi, GPRv

_end:

    mov al, 20h
    out 20h, al

    popState
    iretq

;
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Opcode Exception
_exception6Handler:
    exceptionHandler 6


_int80Handler:
    pushState
    push rbp
    mov rbp, rsp

    push r8
    push rcx
    push rdx
    push rsi
    push rdi
    push rax

    pop rdi ;numSyscall
    pop rsi ;arg0
    pop rdx ;arg1
    pop rcx ;arg2
    pop r8 ;arg3
    pop r9 ;arg4

    call _int80Dispatcher

	mov [aux], rax
    mov rsp, rbp
    pop rbp
    popState
	mov rax, [aux]
    iretq

_timer_tick_handler:
	pushAll

    call timer_handler

	mov rdi, rsp
	call schedule,
	mov rsp, rax

	;signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al
	popAll
	iretq

haltcpu:
	cli
	hlt
	ret


SECTION .bss
	aux resq 1
	GPRv resq 17
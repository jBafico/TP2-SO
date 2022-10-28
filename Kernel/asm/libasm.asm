GLOBAL cpuVendor

GLOBAL getSeconds
GLOBAL getMins
GLOBAL getHour
GLOBAL getKey
GLOBAL getDayOfMonth
GLOBAL getMonth
GLOBAL getYear

GLOBAL prepareRegisters
GLOBAL getByte

GLOBAL restartSCM

EXTERN getStackBase
EXTERN runSampleCodeModule

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid

	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

%macro getClock 1
	push rbp
    mov rbp, rsp

    mov al, %1
    out 70h, al
    in al, 71h

    leave
    ret
%endmacro

getSeconds:
    getClock 0

getMins:
    getClock 2

getHour:
    getClock 4

getDayOfMonth:
    getClock 7

getMonth:
    getClock 8

getYear:
    getClock 9


getKey:
    push rbp
    mov rbp, rsp
    mov rax,0

.loop:
    in al,0x64
    and al,0x01
    cmp al, 0
    je .loop
    in al,0x60

    mov rsp, rbp
    pop rbp
    ret


getByte:
    push rbp
    mov rbp, rsp

    mov rax, 0      ; no se si hace falta
    mov al, [rdi]

    mov rsp, rbp
    pop rbp
    ret

;rax, rbx, rcx, rdx, rbp, rsi, rdi, rsp, r8,r9,r10,r11,r12,r13,r14,r15
prepareRegisters:
    ;mov [GPRv+x*8], rax

    ;no armamos stackframe para preservar registros e imprimir en momento pedido
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

    mov rax, GPRv
    ret

restartSCM:
    call getStackBase	        ; Get thet stack address
	mov rsp, rax				; Set up the stack with the returned address
	call runSampleCodeModule


section .bss
    GPRv resq 17




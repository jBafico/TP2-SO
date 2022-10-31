GLOBAL sysWrite
GLOBAL sysRead
GLOBAL sysTime
GLOBAL sysClearScreen
GLOBAL sysPrintMem
GLOBAL sysInfoReg
GLOBAL sysSleep
GLOBAL sysTask
GLOBAL sysRunTasks

section .data
    SYSREAD equ 0
    SYSWRITE equ 1
    SYSCLEARSCREEN equ 69
    SYSPRINTMEM equ 70
    SYSINFOREG equ 71
    SYSSLEEP equ 72
    SYSMALLOC equ 74
                    SYSFREE equ 75
                    SYSADDPROCESS equ 76
                    SYSWAIT equ 77
                    SYSKILLPROCESS equ 78
                    SYSGETPID equ 79
                    SYSBLOCKPROCESS equ 80,
                    SYSSETSTATE equ 81
                    SYSSETPRIO equ 82
                    SYSREADYPROCESS equ 83
                    SYSSEMWAIT equ 84
                    SYSSEMOPEN equ 85
                    SYSSEMPOST equ 86
                    SYSSEMCLOSE equ 87
                    SYSSEMSTATUS equ 88
                    SYSPIPEOPEN equ 89
                    SYSPIPECLOSE equ 90
                    SYSPIPEWRITE equ 91
                    SYSPIPEREAD equ 92
                    SYSYIELD equ 93
    SYSTIME equ 201

section .text

; uso esta syscall para imprimir caracter en pantalla, aparece con sys

sysWrite:
    push rbp
    mov rbp, rsp
    mov rax, SYSWRITE

    ;en rdi tengo el fileDescriptor ya
    ;en rsi tengo el puntero al caracter
    ;en rdx tengo la longitud

    int 80h
    mov rsp, rbp
    pop rbp
    ret

sysRead:
    push rbp
    mov rbp, rsp
    mov rax, SYSREAD

        ;en rdi tengo el fileDescriptor ya
        ;en rsi tengo el puntero al caracter
        ;en rdx tengo la longitud
    int 80h
    ;tengo en rax, el valor de retorno de la interrupcion
    mov rsp, rbp
    pop rbp
    
    ret

sysTime:
    push rbp
    mov rbp, rsp
    mov rax, SYSTIME
    ;ya tengo en rdi el puntero a la estructura

    int 80h
    mov rsp, rbp
    pop rbp
    ret

sysClearScreen:
    push rbp
    mov rbp, rsp
    mov rax, SYSCLEARSCREEN

    int 80h
    mov rsp, rbp
    pop rbp
    ret

 sysInfoReg:
    push rbp
    mov rbp, rsp
    mov rax, SYSINFOREG

    int 80h
    mov rsp, rbp
    pop rbp
    ret

  sysPrintMem:
      push rbp
      mov rbp, rsp
      mov rax, SYSPRINTMEM

          ;en rdi tengo la direccion de mem, en rsi address

      int 80h
      mov rsp, rbp
      pop rbp
      ret

sysSleep:
    push rbp
    mov rbp, rsp
    mov rax, SYSSLEEP

    int 80h
    mov rsp, rbp
    pop rbp
    ret


sysMalloc:
    push rbp
    mov rbp, rsp
    mov rax, SYSMALLOC

        int 80h
        mov rsp, rbp
        pop rbp
        ret

;void sysFree(void * block);

sysFree:
    push rbp
    mov rbp, rsp
    mov rax, SYSFREE

        int 80h
        mov rsp, rbp
        pop rbp
        ret

;int sysAddProcess(void (*entryPoint)(int, char**), int argc, char** argv, int foreground);

sysAddProcess:
    push rbp
    mov rbp, rsp
    mov rax, SYSADDPROCESS

        int 80h
        mov rsp, rbp
        pop rbp
        ret

;void sysWait(int pid);
sysWait:
push rbp
    mov rbp, rsp
    mov rax, SYSWAIT

        int 80h
        mov rsp, rbp
        pop rbp
        ret

;int sysKillProcess(uint64_t pid);
sysKillProcess:
push rbp
    mov rbp, rsp
    mov rax, SYSKILLPROCESS

        int 80h
        mov rsp, rbp
        pop rbp
        ret
;int sysGetPID();
sysGetPID:
push rbp
    mov rbp, rsp
    mov rax, SYSGETPID

        int 80h
        mov rsp, rbp
        pop rbp
        ret
;int sysBlockProcess(uint64_t pid);
sysBlockProcess:
push rbp
    mov rbp, rsp
    mov rax, SYSBLOCKPROCESS

        int 80h
        mov rsp, rbp
        pop rbp
        ret
;int sysSetState(uint64_t pid, pState newState);
sysSetState:
push rbp
    mov rbp, rsp
    mov rax, SYSSETSTATE

        int 80h
        mov rsp, rbp
        pop rbp
        ret
;void sysSetPriority(uint64_t pid, int newPriority);
sysSetPriority:
push rbp
    mov rbp, rsp
    mov rax, SYSSETPRIO

        int 80h
        mov rsp, rbp
        pop rbp
        ret
;int sysReadyProcess(uint64_t pid);
sysreadyProcess:
push rbp
    mov rbp, rsp
    mov rax, SYSREADYPROCESS

        int 80h
        mov rsp, rbp
        pop rbp
        ret
;int sysSemWait(uint32_t id);
sysSemWait:
push rbp
    mov rbp, rsp
    mov rax, SYSSEMWAIT

        int 80h
        mov rsp, rbp
        pop rbp
        ret
;int sysSemClose(uint32_t id);
sysSemClose:
push rbp
    mov rbp, rsp
    mov rax, SYSSEMCLOSE

        int 80h
        mov rsp, rbp
        pop rbp
        ret
;int sysSemOpen(uint32_t id, uint64_t initialValue);
sysSemOpen:
push rbp
    mov rbp, rsp
    mov rax, SYSSEMOPEN

        int 80h
        mov rsp, rbp
        pop rbp
        ret
;int sysSemPost(uint32_t id);
sysSemPost:
push rbp
    mov rbp, rsp
    mov rax, SYSSEMPOST

        int 80h
        mov rsp, rbp
        pop rbp
        ret
;void sysSemStatus();
sysSemStatus:
push rbp
    mov rbp, rsp
    mov rax, SYSSEMSTATUS

        int 80h
        mov rsp, rbp
        pop rbp
        ret
;int sysPipeOpen(int pipeId);
sysPipeOpen:
push rbp
    mov rbp, rsp
    mov rax, SYSPIPEOPEN

        int 80h
        mov rsp, rbp
        pop rbp
        ret
;int sysPipeClose(int pipeId);
sysPipeClose:
push rbp
    mov rbp, rsp
    mov rax, SYSPIPECLOSE

        int 80h
        mov rsp, rbp
        pop rbp
        ret
;int sysPipeRead(int pipeId);
sysPipeRead:
push rbp
    mov rbp, rsp
    mov rax, SYSPIPEREAD

        int 80h
        mov rsp, rbp
        pop rbp
        ret
;int sysPipeWrite( int pipeId, char * src);
sysPipeWrite:
push rbp
    mov rbp, rsp
    mov rax, SYSPIPEWRITE

        int 80h
        mov rsp, rbp
        pop rbp
        ret
; void sysYield();
sysYield:
push rbp
    mov rbp, rsp
    mov rax, SYSYIELD

        int 80h
        mov rsp, rbp
        pop rbp
        ret

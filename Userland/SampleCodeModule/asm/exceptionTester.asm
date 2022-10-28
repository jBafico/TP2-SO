GLOBAL divideZero
GLOBAL invalidOpCode

section .text

divideZero:
    push rbp
    mov rbp, rsp

    mov rdx, 1
    mov rax, 0
    div rax

    mov rsp, rbp
    pop rbp
    ret



;Obtenido de https://mudongliang.github.io/x86/html/file_module_x86_id_318.html
invalidOpCode:
    push rbp
    mov rbp, rsp

    UD2

    mov rsp, rbp
    pop rbp
    ret
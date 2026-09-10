global _start
extern main

_start:
    call main
    mov  edi, eax
    mov  eax, 60
    syscall
global _start
extern _setup
extern main

_start:
    call _setup
    call main
    mov  edi, eax
    mov  eax, 60
    syscall
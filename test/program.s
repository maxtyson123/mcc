global main
main:
push rbp
mov rbp, rsp
mov eax, 42
jmp _mcc_L0
_mcc_L0:
mov rsp, rbp
pop rbp
ret
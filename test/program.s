global main
main:
push rbp
mov rbp, rsp
mov rax, 2
push rax
push rax
mov rax, 8
push rax
pop rbx
pop rax
mul rax, rbx
push rax
pop rbx
pop rax
add rax, rbx
jmp _mcc_L0
_mcc_L0:
mov rsp, rbp
pop rbp
ret

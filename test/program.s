global main
main:
push rbp
mov rbp, rsp
mov rax, 40
push rax
mov rax, 3
push rax
pop rbx
pop rax
add rax, rbx
push rax
mov rax, 1
push rax
pop rbx
pop rax
sub rax, rbx
jmp _mcc_L0
_mcc_L0:
mov rsp, rbp
pop rbp
ret

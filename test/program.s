section .bss
x:
resq 1
y:
resq 1
section .text
global _setup
_setup:
push rbp
mov rbp, rsp
mov rax, 4
mov  [rel x], rax
mov rax, 3
mov  [rel y], rax
_mcc_L0:
mov rsp, rbp
pop rbp
ret
global main
main:
push rbp
mov rbp, rsp
sub rsp, 16
mov rax, 2
mov [rbp - 8], rax
mov rax, 4
mov [rbp - 16], rax
mov rax, [rel x]
push rax
mov rax, [rel y]
push rax
pop rbx
pop rax
add rax, rbx
push rax
mov rax, [rbp - 8]
push rax
mov rax, [rbp - 16]
push rax
pop rbx
pop rax
add rax, rbx
push rax
pop rbx
pop rax
mul rax, rbx
jmp _mcc_L1
_mcc_L1:
mov rsp, rbp
pop rbp
ret

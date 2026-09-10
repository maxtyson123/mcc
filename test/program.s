section .bss
x:
resq 1
y:
resq 1
z:
resq 1
section .text
global _setup
_setup:
push rbp
mov rbp, rsp
mov rax, 5
mov  [rel x], rax
mov rax, 3
mov  [rel y], rax
mov rax, 2
mov  [rel z], rax
_mcc_L0:
mov rsp, rbp
pop rbp
ret
global main
main:
push rbp
mov rbp, rsp
sub rsp, 24
mov rax, 10
mov [rbp - 8], rax
mov rax, 8
mov [rbp - 16], rax
mov rax, 1
mov [rbp - 24], rax
mov rax, [rel z]
push rax
mov rax, [rel x]
push rax
mov rax, [rbp - 16]
push rax
pop rbx
pop rax
mul rax, rbx
push rax
mov rax, [rbp - 24]
push rax
pop rbx
pop rax
mul rax, rbx
push rax
pop rbx
pop rax
add rax, rbx
jmp _mcc_L1
_mcc_L1:
mov rsp, rbp
pop rbp
ret

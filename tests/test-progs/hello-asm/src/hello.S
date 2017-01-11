# Hello World in Assembly.
# Uses GAS Syntax.
# Taken from: http://cs.lmu.edu/~ray/notes/gasexamples/

.global _start

.text

_start:
    movq $1, %rax
    movq $1, %rdi
    movq $message, %rsi
    movq $13, %rdx
    movq %rax, %r9
    syscall

    movq $60, %rax
    xor %rdi, %rdi
    syscall

message: .ascii "Hello, world!\n"

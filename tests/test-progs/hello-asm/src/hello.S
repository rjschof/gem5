# Hello World in Assembly.
# Uses GAS Syntax.
# Taken from: http://cs.lmu.edu/~ray/notes/gasexamples/

.global _start

.text

_start:
    mov $1, %eax
    mov $1, %edi
    mov $message, %esi
    mov $13, %edx
    syscall

    mov $60, %eax
    xor %edi, %edi
    syscall

message: .ascii "Hello, world!\n"

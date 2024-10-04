# 0 "interrupt_wrappers.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "interrupt_wrappers.S"
# 1 "include/asm.h" 1
# 2 "interrupt_wrappers.S" 2



.globl write; .type write, @function; .align 0; write:
 movl 4(%esp), %edx
 movw 8(%esp), %cx
 movl 12(%esp), %ebx
 movl $4, %eax
 int $0x80
 cmpl $0, %eax
 jge final
 movl %eax, errno
 movl $-1, %eax
final: ret


.globl gettime; .type gettime, @function; .align 0; gettime:
 movl $10, %eax
 int $0x80

 cmpl $0, %eax
 jge final1
 movl %eax, errno
 movl $-1, %eax
final1: ret

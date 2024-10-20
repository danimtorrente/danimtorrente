# 0 "suma.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "suma.S"
# 1 "include/asm.h" 1
# 2 "suma.S" 2

.globl addAsm; .type addAsm, @function; .align 0; addAsm:
 pushl %ebp
 movl %esp, %ebp
 movl 8(%ebp), %eax
 addl 12(%ebp), %eax
 movl %ebp, %esp
 popl %ebp
 ret

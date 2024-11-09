# 0 "interrupt_wrappers.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "interrupt_wrappers.S"
# 1 "include/asm.h" 1
# 2 "interrupt_wrappers.S" 2






.globl write; .type write, @function; .align 0; write:
 pushl %ebp
 movl %esp, %ebp
 pushl %ebx

 movl 8(%ebp), %edx
 movl 12(%ebp), %ecx
 movl 16(%ebp), %ebx

 movl $4, %eax
 int $0x80
 cmpl $0, %eax
 jge final
 movl %eax, errno
 movl $-1, %eax

final: popl %ebx
 movl %ebp, %esp
 popl %ebp
 ret

.globl gettime; .type gettime, @function; .align 0; gettime:
 pushl %ebp
 movl %esp, %ebp

 movl $10, %eax
 int $0x80
 cmpl $0, %eax
 jge final1
 movl %eax, errno
 movl $-1, %eax

final1: movl %ebp, %esp
 popl %ebp
 ret

.globl getpid; .type getpid, @function; .align 0; getpid:
 pushl %ebp
 movl %esp, %ebp

 movl $20, %eax
 int $0x80
        cmpl $0, %eax
        jge final2
        movl %eax, errno
        movl $-1, %eax

final2: movl %ebp, %esp
        popl %ebp
        ret

.globl fork; .type fork, @function; .align 0; fork:
 pushl %ebp
 movl %esp, %ebp

 movl $2, %eax
 int $0x80
        cmpl $0, %eax
 jge final3
        movl %eax, errno
        movl $-1, %eax

final3: movl %ebp, %esp
        popl %ebp
        ret

.globl exit; .type exit, @function; .align 0; exit:
        pushl %ebp
        movl %esp, %ebp

        movl $1, %eax
        int $0x80





final4: movl %ebp, %esp
        popl %ebp
        ret

.globl block; .type block, @function; .align 0; block:
        pushl %ebp
        movl %esp, %ebp

        movl $1, %eax
        int $0x80





final5: movl %ebp, %esp
        popl %ebp
        ret


.globl unblock; .type unblock, @function; .align 0; unblock:
        pushl %ebp
        movl %esp, %ebp
 movl 8(%ebp), %edx

        movl $1, %eax
        int $0x80





final6: movl %ebp, %esp
        popl %ebp
        ret

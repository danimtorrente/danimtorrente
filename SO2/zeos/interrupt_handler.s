# 0 "interrupt_handler.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "interrupt_handler.S"
# 1 "include/asm.h" 1
# 2 "interrupt_handler.S" 2
# 1 "include/entry.h" 1




# 1 "include/segment.h" 1
# 6 "include/entry.h" 2
# 3 "interrupt_handler.S" 2
# 1 "include/exit.h" 1


# 1 "include/asm.h" 1
# 4 "include/exit.h" 2
# 4 "interrupt_handler.S" 2

.globl keyboard_handler; .type keyboard_handler, @function; .align 0; keyboard_handler:
 pushl %gs; pushl %fs; pushl %es; pushl %ds; pushl %eax; pushl %ebp; pushl %edi; pushl %esi; pushl %ebx; pushl %ecx; pushl %edx; movl $0x18, %edx; movl %edx, %ds; movl %edx, %es
 movb $0x20, %al ; outb %al, $0x20 ;
 call keyboard_routine
 movw %es, %edx; movw %ds, %edx; movl %edx, $0x18; popl %edx; popl %ecx; popl %ebx; popl %esi; popl %edi; popl %ebp; popl %eax; popw %ds; popw %es; popl %fs; popl %gs
 iret

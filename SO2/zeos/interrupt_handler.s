# 0 "interrupt_handler.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "interrupt_handler.S"
# 1 "include/asm.h" 1
# 2 "interrupt_handler.S" 2
# 1 "include/segment.h" 1
# 3 "interrupt_handler.S" 2
# 1 "include/entry.h" 1




# 1 "include/segment.h" 1
# 6 "include/entry.h" 2
# 4 "interrupt_handler.S" 2
# 1 "include/exit.h" 1
# 1 "include/asm.h" 1
# 2 "include/exit.h" 2
# 5 "interrupt_handler.S" 2


.globl writeMSR; .type writeMSR, @function; .align 0; writeMSR:
 movl 4(%esp), %ecx
 movl 8(%esp), %eax
 movl 12(%esp), %edx
 wrmsr
 ret


.globl keyboard_handler; .type keyboard_handler, @function; .align 0; keyboard_handler:
 pushl %gs; pushl %fs; pushl %es; pushl %ds; pushl %eax; pushl %ebp; pushl %edi; pushl %esi; pushl %ebx; pushl %ecx; pushl %edx; movl $0x18, %edx; movl %edx, %ds; movl %edx, %es
 movb $0x20, %al ; outb %al, $0x20 ;
 call keyboard_routine
 popl %edx; popl %ecx; popl %ebx; popl %esi; popl %edi; popl %ebp; popl %eax; popl %ds; popl %es; popl %fs; popl %gs
 iret

.globl system_call; .type system_call, @function; .align 0; system_call:
 pushl %gs; pushl %fs; pushl %es; pushl %ds; pushl %eax; pushl %ebp; pushl %edi; pushl %esi; pushl %ebx; pushl %ecx; pushl %edx; movl $0x18, %edx; movl %edx, %ds; movl %edx, %es
 cmpl $0, %EAX
 jl err
 cmpl $MAX_SYSCALL, %EAX
 jg err
 call *sys_call_table(, %EAX, 0x04)
 jmp fin
err: movl $-38, %EAX
fin: movl %EAX, 0x18(%esp)
 popl %edx; popl %ecx; popl %ebx; popl %esi; popl %edi; popl %ebp; popl %eax; popl %ds; popl %es; popl %fs; popl %gs
 iret

.globl syscall_handler_sysenter; .type syscall_handler_sysenter, @function; .align 0; syscall_handler_sysenter:
 push $0x2B
 push %EBP
 pushfl
 push $0x23
 push 4(%EBP)
 pushl %gs; pushl %fs; pushl %es; pushl %ds; pushl %eax; pushl %ebp; pushl %edi; pushl %esi; pushl %ebx; pushl %ecx; pushl %edx; movl $0x18, %edx; movl %edx, %ds; movl %edx, %es
 cmpl $0, %EAX
 jl sysenter_err
 cmpl $MAX_SYSCALL, %EAX
 jg sysenter_err
 call *sys_call_table(, %EAX, 0x04)
 jmp sysenter_fin
sysenter_err:
 movl $-38, %EAX
sysenter_fin:
 movl %EAX, 0x18(%ESP)
 popl %edx; popl %ecx; popl %ebx; popl %esi; popl %edi; popl %ebp; popl %eax; popl %ds; popl %es; popl %fs; popl %gs
 movl (%ESP), %EDX
 movl 12(%ESP), %ECX
 sti
 sysexit

.globl clock_handler; .type clock_handler, @function; .align 0; clock_handler:
 pushl %gs; pushl %fs; pushl %es; pushl %ds; pushl %eax; pushl %ebp; pushl %edi; pushl %esi; pushl %ebx; pushl %ecx; pushl %edx; movl $0x18, %edx; movl %edx, %ds; movl %edx, %es
 movb $0x20, %al ; outb %al, $0x20 ;
 call clock_routine
 popl %edx; popl %ecx; popl %ebx; popl %esi; popl %edi; popl %ebp; popl %eax; popl %ds; popl %es; popl %fs; popl %gs
 iret

.globl page_handler; .type page_handler, @function; .align 0; page_handler:







 call page_routine
 iret

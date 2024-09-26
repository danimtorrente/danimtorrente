/*
 * exit.S - Exit point to user mode from system mode
 */

#include <asm.h>
#include <segment.h>


#define RESTORE_ALL \
	movl %es, %edx \
	movl %ds, %edx \
	movl %edx, $__KERNEL_DS \
	popl %edx \
	popl %ecx \
	popl %ebx \
	popl %esi \
	popl %edi \
	popl %ebp \
	popl %eax \
	popl %ds \
	popl %es \
	popl %fs \
	popl %gs


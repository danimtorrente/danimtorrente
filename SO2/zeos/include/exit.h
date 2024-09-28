

#include "asm.h"
#include "segment.h"

#ifndef __EXIT_H__
#define __EXIT_H__

#define RESTORE_ALL \
	movl %es, %edx; \
	movl %ds, %edx; \
	movl %edx, $__KERNEL_DS; \
	popl %edx; \
	popl %ecx; \
	popl %ebx; \
	popl %esi; \
	popl %edi; \
	popl %ebp; \
	popl %eax; \
	popl %ds; \
	popl %es; \
	popl %fs; \
	popl %gs

#define EOI \
movb $0x20, %al ; \
outb %al, $0x20 ;

#endif

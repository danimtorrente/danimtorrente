#include "asm.h"

// WRAPPERS

ENTRY(write)
	pushl %ebp
	movl %esp, %ebp
	pushl %ebx
////////////////////////////////////////////////
	movl 8(%esp), %edx // fd
	movl 12(%esp), %ecx // buffer
	movl 16(%esp), %ebx // size
///////////////////////////////////////////////
	movl $4, %eax // put identifier of the syscall in eax
	int $0x80 // generate trap
	cmpl $0, %eax // process result
	jge final
	movl %eax, errno
	movl $-1, %eax
//////////////////////////////////////////////////////
final:	popl %ebx
	movl %ebp, %esp
	popl %ebp
	ret //return

ENTRY(gettime)
	pushl %ebp
	movl %esp, %ebp
//////////////////////////////////////////////////
	movl $10, %eax // put identifier of the syscall in eax
	int $0x80 // generate trap
	cmpl $0, %eax // process result
	jge final1
	movl %eax, errno // errno es una etiqueta de memoria
	movl $-1, %eax
/////////////////////////////////////////
final1:	movl %ebp, %esp
	popl %ebp
	ret //return

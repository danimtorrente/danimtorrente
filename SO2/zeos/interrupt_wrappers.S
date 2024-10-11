#include "asm.h"

// WRAPPERS

ENTRY(write)
	pushl %ebp
	movl %esp, %ebp
	pushl %ebx
////////////////////////////////////////////////
	movl 8(%ebp), %edx // fd
	movl 12(%ebp), %ecx // buffer
	movl 16(%ebp), %ebx // size
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

/*
ENTRY(write) // WRITE WITH FAST CALL WRAPPER
	pushl %ebp
	movl %esp, %ebp
	pushl %ebx
	movl 8(%ebp), %edx // fd
	movl 12(%ebp), %ecx // buffer
	movl 16(%ebp), %ebx // size
	movl $4, %eax // put identifier of the syscall in eax
////////////////////////////////////////////////
	//pushl %edx 
	//pushl %ecx
	//pushl etiq
///////////////////////////////////////////////
	//pushl %ebp
	//movl %esp, %ebp
	sysenter
////////////////////////////////////////////////////////
etiq:
	//popl %ebp
	//popl etiq
	//popl %ecx
	//popl %edx
/////////////////////////////////////////////////////////
	cmpl $0, %eax // process result
	jge final
	movl %eax, errno
	movl $-1, %eax
//////////////////////////////////////////////////////
final:	
	popl %ebx
	movl %ebp, %esp
	popl %ebp
	ret //return
*/

#include "asm.h"

// WRAPPERS

// EL SEGON WRITE FUNCIONA AMB SYSENTER, PER VEURE QUE FUNCIONA SIMPLEMENT DESCOMENTAR


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
	pushl %esi
	pushl %edi
////////////////////////////////////////////////
	movl $4, %eax
	movl 8(%ebp), %edx // fd
	movl 12(%ebp), %ecx // buffer
	movl 16(%ebp), %ebx // size
///////////////////////////////////////////////
	pushl %ecx
	pushl %edx

	push $etiq

	push %ebp
	movl %esp, %ebp
	sysenter
////////////////////////////////////////////////////////
etiq:
	popl %ebp
	popl %edx // quitar valor etiqueta, tambien probar: addl $4, %esp
	popl %edx
	popl %ecx

	popl %edi
	popl %esi
	popl %ebx
/////////////////////////////////////////////////////////
	cmpl $0, %eax // process result
	jge final
	movl %eax, errno
	movl $-1, %eax
//////////////////////////////////////////////////////
final:
	movl %ebp, %esp
	popl %ebp
	ret //return
*/

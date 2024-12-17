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

ENTRY(getpid)
	pushl %ebp
	movl %esp, %ebp
//////////////////////////////////////////////////
	movl $20, %eax
	int $0x80
        cmpl $0, %eax // process result
        jge final2
        movl %eax, errno // errno es una etiqueta de memoria
        movl $-1, %eax
/////////////////////////////////////////
final2: movl %ebp, %esp
        popl %ebp
        ret //return

ENTRY(fork)
	pushl %ebp
	movl %esp, %ebp
////////////////////////////////////////////////////
	movl $2, %eax
	int $0x80
        cmpl $0, %eax
	jge final3
        movl %eax, errno // errno es una etiqueta de memoria
        movl $-1, %eax
/////////////////////////////////////////
final3: movl %ebp, %esp
        popl %ebp
        ret //return

ENTRY(exit)
        pushl %ebp
        movl %esp, %ebp
//////////////////////////////////////////////////
        movl $1, %eax
        int $0x80
//        cmpl $0, %eax // process result
//        jge final4
//        movl %eax, errno // errno es una etiqueta de memoria
//        movl $-1, %eax
/////////////////////////////////////////
final4: movl %ebp, %esp
        popl %ebp
        ret //return

ENTRY(block) // void block(void) // entrada 5
        pushl %ebp
        movl %esp, %ebp
//////////////////////////////////////////////////
        movl $1, %eax // numero llamada a sistema (CAMBIAR)
        int $0x80
//        cmpl $0, %eax // process result
//        jge final5
//        movl %eax, errno // errno es una etiqueta de memoria
//        movl $-1, %eax
/////////////////////////////////////////
final5: movl %ebp, %esp
        popl %ebp
        ret //return


ENTRY(unblock) // void unblock(int pid) // entrada 6
        pushl %ebp
        movl %esp, %ebp
	movl 8(%ebp), %edx // pid
//////////////////////////////////////////////////
        movl $1, %eax // numero llamada a sistema (CAMBIAR)
        int $0x80
//        cmpl $0, %eax // process result
//        jge final6
//        movl %eax, errno // errno es una etiqueta de memoria
//        movl $-1, %eax
/////////////////////////////////////////
final6: movl %ebp, %esp
        popl %ebp
        ret //return

ENTRY(quickfork)
	pushl %ebp
        movl %esp, %ebp
//        movl 8(%ebp), %edx // pid
//////////////////////////////////////////////////
//        movl $1, %eax
        int $200
        cmpl $0, %eax // process result
        jge final7
        movl %eax, errno // errno es una etiqueta de memoria
        movl $-1, %eax
/////////////////////////////////////////
final7: movl %ebp, %esp
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

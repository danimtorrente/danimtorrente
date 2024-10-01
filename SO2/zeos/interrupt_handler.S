#include "asm.h"
#include "entry.h"
#include "exit.h"

//HANDLERS

ENTRY(keyboard_handler)
	SAVE_ALL // Mirar si guarda el contexto para interrupciones igual que para escepciones
	EOI
	call keyboard_routine
	RESTORE_ALL
	iret

ENTRY(system_call)
	SAVE_ALL
	// CHECKS
	call sys_write
	// 24(esp)<- EAX
	RESTORE_ALL
	iret

// WRAPPERS

ENTRY(write)
	movl fd, %edx // Parameter passing
	movb buffer, %ecx
	movl size, %ebx
	movl $4, %eax // put identifier of the syscall in eax
	int $0x80 // generate trap
	cmpl $0, %eax // process result
	jge final
	movl %eax, %errno // REVISAR SI ESTA BIEN ESCRITO EL REGISRO
	movl $-1, %eax
final:	ret //return















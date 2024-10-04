#include "asm.h"

// WRAPPERS
// GUARDAR Y RESTAURAR REGISTROS Y ENLACE DINAMICO (ESTANDARES DE C)
ENTRY(write)
	movl 4(%esp), %edx // fd
	movw 8(%esp), %cx // buffer
	movl 12(%esp), %ebx // size
	movl $4, %eax // put identifier of the syscall in eax
	int $0x80 // generate trap
	cmpl $0, %eax // process result
	jge final
	movl %eax, errno
	movl $-1, %eax
final:	ret //return

// GUARDAR Y RESTAURAR REGISTROS Y ENLACE DINAMICO
ENTRY(gettime)
	movl $10, %eax // put identifier of the syscall in eax
	int $0x80 // generate trap
	// El restultado no esta en esp + 0 porque se supone que lo he puesto en eax en la llamada a sistema
	cmpl $0, %eax // process result
	jge final1
	movl %eax, errno // errno es una etiqueta de memoria
	movl $-1, %eax
final1:	ret //return

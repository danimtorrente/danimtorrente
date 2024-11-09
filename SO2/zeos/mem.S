#include "asm.h"

#ifndef __MEM_H__
#define __MEM_H__

// 1) saves the registers ESI, EDI and EBX
// 2) calls the inner_task_switch routine
// 3) restores the previously saved registers


// Paso parametros por la pila, no me queda claro cuando se usan registros para los parametros y cuando pila
ENTRY(task_switch) // void task_switch(union task_union*t);
	pushl %ebp
	movl %esp, %ebp
	pushl %esi
	pushl %edi
	pushl %ebx
	pushl 8(%ebp) // push parametro
	call inner_task_switch
	addl $4, %esp // elimino parametro de la pila
	popl %ebx
	popl %edi
	popl %esi
	popl %ebp
	ret // exit

ENTRY(get_ebp) // REVISAR 
	movl %ebp, %eax
	ret

ENTRY(set_esp) // REVISAR
	movl 4(%esp), %esp
	popl %ebp
	ret

#endif

#include "asm.h"

#ifndef __MEM_H__
#define __MEM_H__


// store in the stack cpu_idle();
// store in the stack initial value of ebp (for example 0)
// save position of the stack(in a new field in task_struct) where we have stored the initial value for the ebp register
// CREO QUE HAY QUE PONER UN EOI O ALGO PARA HACER ENABLE INTERRUPTS
ENTRY(idle_prep) // idle_prep(&cpu_idle(), 0);
	push %ebp
	movl %esp, %ebp

	movl %ebp, %esp
	popl %ebp
	ret // eso hace falta?

/* // DIRIA QUE NO NECESARIO POR AHORA ALMENOS
ENTRY(init_prep)
        push %ebp
        movl %esp, %ebp

        movl %ebp, %esp
        popl %ebp
        ret // eso hace falta?
*/


// store the value of ebp in the PCB
// store in esp the value stored in the new PCB
// restore the ebp register from the stack
// return using ret to this funtion (not always return to this functino but most of the time)
ENTRY(task_switch_ass)
        push %ebp
        movl %esp, %ebp

        movl %ebp, %esp
        popl %ebp
        ret // eso hace falta?



#endif

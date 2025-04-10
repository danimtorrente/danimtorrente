#include "asm.h"
#include "segment.h"
#include "entry.h"
#include "exit.h"

// FAST SYS_CALLS INIT
ENTRY(writeMSR) // two parameters: MSRnum, value to store in that MSR
	movl 4(%esp), %ecx
	movl 8(%esp), %eax
	movl 12(%esp), %edx
	wrmsr
	ret

//HANDLERS
ENTRY(keyboard_handler)
	SAVE_ALL
	EOI
	call keyboard_routine
	RESTORE_ALL
	iret

ENTRY(system_call) // handler llamadas lentas a sistema
	SAVE_ALL
	cmpl $0, %EAX
	jl err
	cmpl $MAX_SYSCALL, %EAX
	jg err
	call *sys_call_table(, %EAX, 0x04)
	jmp fin
err:	movl $-38, %EAX
fin:	movl %EAX, 0x18(%esp)
	RESTORE_ALL
	iret

ENTRY(syscall_handler_sysenter) //PUSH SENSE TIPUS DE DADES?
	push $__USER_DS
	push %EBP // User stack address
	pushfl
	push $__USER_CS
	push 4(%EBP) // User return address
	SAVE_ALL
	cmpl $0, %EAX
	jl sysenter_err
	cmpl $MAX_SYSCALL, %EAX
	jg sysenter_err
	call *sys_call_table(, %EAX, 0x04)
	jmp sysenter_fin
sysenter_err:
	movl $-38, %EAX
sysenter_fin:
	movl %EAX, 0x18(%ESP)
	RESTORE_ALL
	movl (%ESP), %EDX // Return address
	movl 12(%ESP), %ECX // User stack address
	sti // Enable interrupts again
	sysexit

ENTRY(clock_handler)
	SAVE_ALL
	EOI
	call clock_routine
	RESTORE_ALL
	iret

ENTRY(page_handler) // PASAR PARAMETRO ERROR Y DIRECCION EIP QUE SE ACCEDE A TRAVES DE LA PILA
// AL FER AIXO EM DONAVA ERROR, PER QUE? COM QUE ES PAGE FAULT NO PUC MODIFICAR RES DE MEMORIA INCLOS LA PILA?
//	pushl %ebp
//	movl %esp, %ebp
// NO ENTENC PERQUE FUNCIONA SENSE LES SEGUENTS DUES INSTRUCCIONS, PENSAVA QUE L'ERROR I LA DIRECCIO ES PASSAVA A LA PILA
// I NO A REGISTRES
//	movl (%ebp), %edx // mover primer elemento de la pila como parametro que es el error
//	movl 4(%ebp), %ecx  // mover segundo elemento de la pila como paramtero que es la direccion (eip)
	call page_routine
	iret

ENTRY(quickfork_handler)
        SAVE_ALL
        EOI
        call quickfork_routine
        RESTORE_ALL
        iret


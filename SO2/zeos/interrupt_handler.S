#include "asm.h"
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

ENTRY(system_call)
	SAVE_ALL
	cmpl $0, %EAX
	jl err
	cmpl $MAX_SYSCALL, %EAX
	jg err
	call *sys_call_table(, %EAX, 0x04)
	jmp fin
err:	movl $-38, %EAX // ENOSYS ERROR: CONSULTAR NUMERO ERROR
fin:	movl %EAX, 0x18(%esp)
	RESTORE_ALL
	iret

ENTRY(syscall_handler_sysenter) //pillat directament de la practica, puc fer push sense tipus de dades?
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
	movl $-38, %EAX // ENOSYS ERROR: CONSULTAR NUMERO ERROR si es 38 definir variable enosys
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

// WRAPPERS

ENTRY(write)
	movl 4(%esp), %edx // fd
	movw 8(%esp), %cx // buffer
	movl 12(%esp), %ebx // size
	movl $4, %eax // put identifier of the syscall in eax
	int $0x80 // generate trap
	// AQUI EL RESULTADO DE INT DEBERIA ESTAR EN ESP + 0 NO?????
	// movl (%esp), %eax
	cmpl $0, %eax // process result
	jge final
	// movl %eax, %errno // CONSULTAR COMO PASAR EL VALOR DE ERROR A ERRNO
	movl $-1, %eax
final:	ret //return

ENTRY(gettime)
	movl $10, %eax // put identifier of the syscall in eax
	int $0x80 // generate trap
	// AQUI EL RESULTADO DE INT DEBERIA ESTAR EN ESP + 0 NO?????
	// movl (%esp), %eax
	cmpl $0, %eax // process result
	jge final1
	// movl %eax, %errno // CONSULTAR COMO PASAR EL VALOR DE ERROR A ERRNO
	movl $-1, %eax
final1:	ret //return












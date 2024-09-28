#include "asm.h"
#include "entry.h"
#include "exit.h"

ENTRY(keyboard_handler)
	SAVE_ALL // Mirar si guarda el contexto para interrupciones igual que para escepciones
	EOI
	call keyboard_routine
	RESTORE_ALL
	iret

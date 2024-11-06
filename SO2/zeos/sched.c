/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>

union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));

#if 0
struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}
#endif

extern struct list_head blocked; // NO ENTIENDO QUE SENTIDO TIENE HACER EXTERN EN EL .C SIN DECLARAR 
				 // LA VARIABLE EN EL ARCHIVO .H
/////////////////////////////////////////////////////////////////////////////////////////////////////////
extern struct list_head freequeue;
extern struct list_head readyqueue;
void idle_prep(); // en mem.S ----- ¡¡ LA CABECERA TIENE PARAMETROS !!
void init_prep(); // en mem.S
struct task_struct * idle_task;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t) 
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t) 
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}


int allocate_DIR(struct task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");

	while(1)
	{
	;
	}
}

void init_idle (void) // SE EJECUTA SIEMPRE EN MODO SISTEMA
{
// ESTO DEBERIA SER STRUCT O UNION? PORQUE FREEQUEUE TIENE LISTAS QUE ESTAN CONTENIDAS EN EL TASK UNION TAMBIEN
// GET AN AVAILABLE TASK_UNION FROM FREEQUEUE
struct list_head *first_elem = list_first(freequeue);
struct task_struct idle = list_entry(first_elem, task_struct, list);
// BORRAR FIRST_ELEM DE LA FREEQUEUE

// PID = 0
idle->PID = 0;

// INIT dir_pages_baseAaddr with a new directory  to store the process address space using allocate_DIR
idle->dir_pages_baseAddr = allocate_DIR(idle);

// call a .S function to rellocate the stack? PASAR VALORES COMO PARAMETROS Y AÑADIRLO EN ASSEMBLER
idle_prep(&cpu_idle(), 0);
// FALTA PONER idle->k_esp = (el valor de esp que no se como conservarlo)
// QUE POR CIERTO SI PONGO LA DIRECCION DE RETORNO CPU_IDLE NUNCA EJECUTA ESTA PARTE DEL CODIGO NO?
idle_task = idle; // INIT idle_task
// HABRIA QUE HACER UN EOI AQUI O EN IDLE_PREP PARA QUE PUEDAN LLEGAR INTERRUPCIONES MIENTRAS SE EJECUTA?
// añadirla a la READYQUEUE? NO NO?
}

void init_task1(void)
{

// ESTO DEBERIA SER STRUCT O UNION? PORQUE FREEQUEUE TIENE LISTAS QUE ESTAN CONTENIDAS EN EL TASK UNION TAMBIEN
// GET AN AVAILABLE TASK_UNION FROM FREEQUEUE
struct list_head *first_elem = list_first(freequeue);
struct task_struct init = list_entry(first_elem, task_struct, list);
// BORRAR FIRST_ELEM DE LA FREEQUEUE

// PID = 1
init->PID = 1;

// INIT dir_pages_baseAaddr with a new directory  to store the prcess address space using allocate_DIR
init->dir_pages_baseAddr = allocate_DIR(idle);

// Complete the init of address space using set_user_pages (mm.c).
set_user_pages(init);

// Update Tss for int and MSR for sysenters. ASI????
tss.esp0 = init->dir_pages_baseAddr;
writeMSR(0x175, tss.esp0, 0);
//UPDATE INIT->k_esp??????????????????????????????

// set_cr3(page_table_entry *dir) [mm.c] function
set_cr3(init->dir_pages_base_Addr);
}


void init_sched()
{
	INIT_LIST_HEAD(&freequeue); // init free queue
	for (int i = 0; i < NR_TASKS; ++i) list_add(&(task[i].list), &freequeue); // add tasks to free queue
	INIT_LIST_HEAD(&readyqueue); // init ready queue
}

struct task_struct* current()
{
  int ret_value;
  
  __asm__ __volatile__(
  	"movl %%esp, %0" // el %0 es un marcador que se sustituira por ret_value
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}

void task_switch(union task_union*t) {
// modify tss.esp0
// modify MSR(0x175,....)
// USE set_cr3 to point to the page directory of the new task

// call a .S function to rellocate the stack???
}

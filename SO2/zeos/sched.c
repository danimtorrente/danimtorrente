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

extern struct list_head blocked; // POR QUE NO ESTA DEFINIDO EN EL .H PERO SI TIENE EL EXTERN?
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern struct list_head freequeue;
extern struct list_head readyqueue;
void idle_prep(); // en mem.S
void init_prep(); // en mem.S
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
idle.PID = 0;

//usar allocate_DIR
// INIT dir_pages_baseAaddr with a new directory  to store the prcess address space using allocate_DIR

//llamar a idle_prep();
// call a .S function to rellocate the stack?

// inicializar la task_struct idle_task
// init idle_task
//añadirla a la READYQUEUE
}

void init_task1(void)
{
/*
// PID = 1
// INIT dir_pages_baseAaddr with a new directory  to store the prcess address space using allocate_DIR
// Complete the init of address space using set_user_pages (mm.c).
// Update Tss for int and MSR for sysenters
// set_cr3(page_table_entry *dir) [mm.c] function
*/
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
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}


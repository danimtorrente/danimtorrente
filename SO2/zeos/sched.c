/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>

union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));

// #if 0 // Supongo que estoy hay que descomentarlo? o usar list_entry directamente?
struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}
// #endif

struct list_head blocked;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
struct list_head freequeue;
struct list_head readyqueue;
void idle_prep(); // en mem.S ----- ¡¡ LA CABECERA TIENE PARAMETROS !!
void init_prep(); // en mem.S
struct task_struct * idle_task;
int pids;
extern int quantum_left;
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
// GET AN AVAILABLE TASK_UNION FROM FREEQUEUE
struct list_head *first_elem = list_first(&freequeue);
list_del(first_elem);
union task_union *idle = (union task_union*)list_head_to_task_struct(first_elem);

// PID = 0
idle->task.PID = 0;

// INIT dir_pages_baseAaddr with a new directory  to store the process address space using allocate_DIR
allocate_DIR(&idle->task);

idle->stack[KERNEL_STACK_SIZE - 1] = (unsigned long)cpu_idle; // el nombre de una funcion ya es su posicion de memoria
idle->stack[KERNEL_STACK_SIZE - 2] = 0;
idle->task.kernel_esp = &(idle->stack[KERNEL_STACK_SIZE - 2]);
///////////////////////////////////////////////////////////////////////////////////////////////
idle->task.parent = NULL;
INIT_LIST_HEAD(&idle->task.children);
idle_task = (struct task_struct*)idle;
}

void init_task1(void)
{
// GET AN AVAILABLE TASK_UNION FROM FREEQUEUE
struct list_head *first_elem = list_first(&freequeue);
list_del(first_elem);
union task_union *init = (union task_union*)list_head_to_task_struct(first_elem);

// PID = 0
init->task.PID = 1;

// INIT dir_pages_baseAaddr with a new directory  to store the process address space using allocate_DIR
allocate_DIR(&init->task);

// Complete the init of address space using set_user_pages (mm.c).
set_user_pages(&init->task);

// Quantum stuff
init->task.quantum = DEFAULT_QUANTUM;
quantum_left = init->task.quantum;

// Update Tss for int and MSR for sysenters.
init->task.kernel_esp = &(init->stack[KERNEL_STACK_SIZE]); // NECESARIO?
// ACCEDE A LA POSICION DE MEMORIA DESPUES DEL ULTIMO ELEMENTO, AUNQUE NO EXISTA SE PUEDE HACER SOLO PARA SACAR
// LA POSICION DE MEMORIA, NO PARA ACCEDER AL ELEMENTO PORQUE ESE ELEMENTO NO EXISTE
tss.esp0 = (DWord)init->task.kernel_esp;
writeMSR(0x175, tss.esp0, 0);

// set_cr3(page_table_entry *dir) [mm.c] function
set_cr3(init->task.dir_pages_baseAddr);
//////////////////////////////////////////////////////////////////////////////////////////////////
init->task.parent = idle_task;  // El padre de init es idle
INIT_LIST_HEAD(&init->task.children);
// Agregar init a la lista de hijos de idle
list_add_tail(&init->task.sibling, &idle_task->children);

}


void init_sched()
{
	pids = 2;
	INIT_LIST_HEAD(&freequeue); // init free queue
	for (int i = 0; i < NR_TASKS; ++i) list_add(&(task[i].task.list), &freequeue); // add tasks to free queue
	INIT_LIST_HEAD(&readyqueue); // init ready queue
	INIT_LIST_HEAD(&blocked); // init blocked queue
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

void inner_task_switch(union task_union*t) {

// modify tss.esp0
  tss.esp0 = KERNEL_ESP(t);

// modify MSR(0x175,....)
  writeMSR(0x175, tss.esp0, 0); // ultimo parametro necesario?

// USE set_cr3 to point to the page directory of the new task
  set_cr3(get_DIR((struct task_struct*)t));

// Store de current value of the ebp register in the pcb
  current()->kernel_esp = (unsigned long *)get_ebp(); //REVISAR

// Set esp register to point to the stored valure in the new pcb
  set_esp(t->task.kernel_esp); //REVISAR
}

int get_quantum(struct task_struct *t) 
{
    return t->quantum;
}

void set_quantum(struct task_struct *t, int new_quantum) 
{
    t->quantum = new_quantum;
}

void update_sched_data_rr (void)
{
 --quantum_left;
}

int needs_sched_rr (void)
{
  if (quantum_left == 0 && !list_empty(&readyqueue)) return 1;

  if (quantum_left == 0) quantum_left = get_quantum(current());

  return 0;
}

void update_process_state_rr (struct task_struct *t, struct list_head *dst_queue)
{
  if (t != current() && t != idle_task) list_del(&t->list);
  if (dst_queue != NULL) list_add_tail(&t->list, dst_queue); // REVISAR
}

void sched_next_rr (void)
{
  struct task_struct *next_task;

  if (list_empty(&readyqueue)) next_task = idle_task;
  else next_task = list_head_to_task_struct(list_first(&readyqueue));

  update_process_state_rr(next_task, NULL);

  task_switch((union task_union*)next_task);
}

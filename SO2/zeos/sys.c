/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#include "interrupt.h"

#define LECTURA 0
#define ESCRIPTURA 1

int quantum_left;

int check_fd(int fd, int permissions)
{
  if (fd!=1) return -9; /*EBADF*/
  if (permissions!=ESCRIPTURA) return -13; /*EACCES*/
  return 0;
}

int sys_ni_syscall()
{
	return -38; /*ENOSYS*/
}

int sys_getpid()
{
	return current()->PID;
}

int ret_from_fork() { // MIRAR SI ESTO SOLO TA BIEN
	return 0;
}

int sys_fork()
{
    if (list_empty(&freequeue)) {
        return -1; // DEFINIR ENOMEM ERROR
    }

    struct list_head *free = list_first(&freequeue);
    list_del(free);
    union task_union *pcb = (union task_union *)list_head_to_task_struct(free);

    // Copiar el task_union del padre al hijo
    copy_data(current(), pcb, sizeof(union task_union));

    // Asignar un nuevo directorio de páginas al hijo
    allocate_DIR(&(pcb->task));

    page_table_entry *PT_parent = get_PT(current());
    page_table_entry *PT_child = get_PT(&(pcb->task));

    // Asignar marcos para las páginas de datos del hijo
    int pag, frame;
    for (pag = PAG_LOG_INIT_DATA; pag < PAG_LOG_INIT_DATA + NUM_PAG_DATA; pag++) {
        frame = alloc_frame();
        if (frame != -1) {
            set_ss_pag(PT_child, pag, frame);
        } else {
            for (int i = PAG_LOG_INIT_DATA; i < pag; i++) {
                free_frame(get_frame(PT_child, i));
                del_ss_pag(PT_child, i);
            }
            list_add_tail(free, &freequeue);
            return -1; // DEFINIR ENOMEM ERROR
        }
    }

    // Copiar las entradas de la tabla de páginas para el código y datos del sistema
    for (pag = 0; pag < NUM_PAG_KERNEL; pag++) { // REVISAR
        set_ss_pag(PT_child, pag, get_frame(PT_parent, pag));
    }
    for (pag = PAG_LOG_INIT_CODE; pag < PAG_LOG_INIT_CODE + NUM_PAG_CODE; pag++) { // REVISAR
        set_ss_pag(PT_child, pag, get_frame(PT_parent, pag));
    }

    int NUM_PAG_USER = NUM_PAG_CODE + NUM_PAG_DATA;

    // Copiar las páginas de datos del padre al hijo
    for (pag = PAG_LOG_INIT_DATA; pag < PAG_LOG_INIT_DATA + NUM_PAG_DATA; pag++) {
        set_ss_pag(PT_parent, pag + NUM_PAG_USER, get_frame(PT_child, pag));
        set_cr3(get_DIR(current()));  // Vaciar TLB

        copy_data((void *)(pag << 12), (void *)((pag + NUM_PAG_USER) << 12), PAGE_SIZE);

        del_ss_pag(PT_parent, pag + NUM_PAG_USER);
        set_cr3(get_DIR(current()));  // Vaciar TLB
    }

    // Preparar el stack del hijo
    pcb->task.kernel_esp = &(pcb->stack[KERNEL_STACK_SIZE - 19]);
    pcb->stack[KERNEL_STACK_SIZE - 19] = 0;  // Valor de retorno (EAX)
    pcb->stack[KERNEL_STACK_SIZE - 18] = (DWord)ret_from_fork;  // Dirección de retorno
/*
    // Asignar PID al hijo
    pcb->task.PID = pids;
    ++pids;
*/
///////////////////////////////////////////////////////////////////////////////////////// 
// SI A PARTIR DE AQUI ESTA BIEN ES UN TRIPLE DES DE FUERA DEL ESTADIO

    // Asignar un nuevo PID al hijo
    pcb->task.PID = ++pids;

    // Establecer el padre del hijo
    pcb->task.parent = current();

    // Inicializar la lista de hijos del hijo
    INIT_LIST_HEAD(&pcb->task.children);

    // Agregar el hijo a la lista de hijos del padre
    list_add_tail(&pcb->task.sibling, &current()->children);

///////////////////////////////////////////////////////////////////////////////////////////////
    // Agregar el proceso hijo a la readyqueue
    list_add_tail(&(pcb->task.list), &readyqueue);

    return pcb->task.PID;
}

void sys_exit()
{

    struct task_struct *curr = current();

    // Eliminar el proceso actual de la lista de hijos de su padre
    if (curr->parent != NULL) {
        list_del(&curr->sibling);
    }

    // Mover los hijos vivos al proceso idle
    struct list_head *pos, *n;
    list_for_each_safe(pos, n, &curr->children) {
        struct task_struct *child = list_entry(pos, struct task_struct, sibling);
        // Actualizar el padre del hijo al proceso idle
        child->parent = idle_task;
        // Mover el hijo a la lista de hijos del proceso idle
        list_del(&child->sibling);
        list_add_tail(&child->sibling, &idle_task->children);
    }

    // Liberar los recursos del proceso actual
    free_user_pages(curr); // REVISAR WARNING AL USAR ESTA FUNCION
    update_process_state_rr(curr, &freequeue);
    sched_next_rr();

}


// fd = file descriptor. In this delivery always 1
// buffer = pointer to the bytes
// size = number of bytes
int sys_write(int fd, char * buffer, int size) {
	int res = 0;
	res = check_fd(fd, 1);
	if (buffer == NULL) res = -1;// CODIGO ERROR?
	if (size < 0) res = -1; // CODIGO ERROR?

	// CREAR VECTOR Y COPIAR CACHO A CACHO DE USUARIO A MEMORIA
	copy_from_user(buffer, buffer+size, size); // REVISAR
	if (res >= 0) {
		res = sys_write_console(buffer, size);
	}
	copy_to_user(buffer, buffer+size, size); // REVISAR

	return res;
}


int sys_gettime() {
	return zeos_ticks;
}

void sys_block() // 5
{
    if (current()->pending_unblocks > 0) {
        // Hay desbloqueos pendientes, disminuir el contador y continuar
        current()->pending_unblocks--;
    } else {
        // No hay desbloqueos pendientes, bloquear el proceso actual
        current()->state = ST_BLOCKED;
        update_process_state_rr(current(), &blocked);
        sched_next_rr();
    }
}

int sys_unblock(int pid) // 6
{
    // Buscar el proceso con el PID dado en la lista de hijos del proceso actual
    struct list_head *pos;
    struct task_struct *child = NULL;
    list_for_each(pos, &current()->children) {
        struct task_struct *temp = list_entry(pos, struct task_struct, sibling);
        if (temp->PID == pid) {
            child = temp;
            break;
        }
    }

    if (child == NULL) {
        // El proceso con el PID dado no es hijo del proceso actual
        return -1;
    }

    if (child->state == ST_BLOCKED) {
        // El proceso está bloqueado, moverlo a la readyqueue
        child->state = ST_READY;
        list_del(&child->list);            // Eliminar de la cola de bloqueados
        list_add_tail(&child->list, &readyqueue);
        return 0;
    } else {
        // El proceso no está bloqueado, incrementar su contador de desbloqueos pendientes
        child->pending_unblocks++;
        return 0;
    }
}

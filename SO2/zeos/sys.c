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

int sys_fork()
{
  int PID=-1;

  // creates the child process
  
  return PID;
}

void sys_exit()
{  
}

// fd = file descriptor. In this delivery always 1
// buffer = pointer to the bytes
// size = number of bytes
int sys_write(int fd, char * buffer, int size) {
	int res = 0;
	res = check_fd(fd, 1);
	if (buffer == NULL) res = -1;// CODIGO ERROR?
	if (size < 0) res = -1; // CODIGO ERROR?
	// copy_from_user(buffer, *buffer+size???, size)
	if (res >= 0) {
		res = sys_write_console(buffer, size);
	}
	//copy_to_user(buffer, *buffer+size???, size)
	return res;
}


int sys_gettime() {
	return zeos_ticks;
}

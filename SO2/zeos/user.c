#include <libc.h>
#include "wrappers.h"

char buff[24];

int pid;

int add(int par1, int par2) {
	return par1+par2;
}

int addAsm(int par1, int par2);

int getpid();
int gettime();
int write(int df, char * buffer, int size);

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

//  char* p = 0;
//  *p = "x";

  int bytes_written = write(1, "\n", 1);
  if (bytes_written == -1) {
	perror();
  }

//  int aux = add(0x42, 0x666);
  int aux = getpid();
  itoa(aux, buff);
  int len = strlen(buff);
//  write(1, &buff[0], len);

//  int aux2 = addAsm(0x42, 0x666);

  bytes_written = write(1, &buff[0], len);
  if (bytes_written == -1) {
	perror();
  }
  while(1) { }
}

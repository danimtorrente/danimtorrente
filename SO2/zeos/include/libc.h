/*
 * libc.h - macros per fer els traps amb diferents arguments
 *          definició de les crides a sistema
 */
 
#ifndef __LIBC_H__
#define __LIBC_H__

#include <misc.h>
#include <stats.h>

int write(int fd, char *buffer, int size);


void itoa(int a, char *b);
/*
AHORA ESTAN DEFINIDAS EN MISC.H

int strlen(char *a);
*/
int getpid();

int fork();

void exit();

void perror(void);

#endif  /* __LIBC_H__ */

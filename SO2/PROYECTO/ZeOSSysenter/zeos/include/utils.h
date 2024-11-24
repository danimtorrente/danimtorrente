#ifndef UTILS_H
#define UTILS_H

void copy_data(void *start, void *dest, int size);
int copy_from_user(void *start, void *dest, int size);
int copy_to_user(void *start, void *dest, int size);

#define VERIFY_READ	0
#define VERIFY_WRITE	1
int access_ok(int type, const void *addr, unsigned long size);

#define min(a,b)	(a<b?a:b)

unsigned long get_ticks(void);

void memset(void *s, unsigned char c, int size);

#define BUFFER_SIZE 20

/* KEYBOARD BUFFER VARIABLES */
#define FULL 0
#define OKAY 1
#define EMPTY -1
//IGUAL DEBERIA HACER UN STRUCT
extern char * headBuffer;
extern char * queueBuffer;
extern char bufferTeclat[BUFFER_SIZE];

#define NOTWAITING -1
extern int bufferTimeout;
extern int getKeyPid;
/* KEYBOARD BUFFER METHODS */

int checkBuffer(); // reutrns if full, okay or empty
void insertChar(char c); // mirar si es mejor usando puntero, diria k no
char deleteChar();
void init_game_func();

/* CRIDES A SISTEMA */
void sys_block();
int sys_unblock(int pid);

#endif

/*
 * entry.h - Definició del punt d'entrada de les crides al sistema
 */

#include "segment.h"

#ifndef __ENTRY_H__
#define __ENTRY_H__


#define SAVE_ALL \
      pushl %gs; \
      pushl %fs; \
      pushl %es; \
      pushl %ds; \
      pushl %eax; \
      pushl %ebp; \
      pushl %edi; \
      pushl %esi; \
      pushl %ebx; \
      pushl %ecx; \
      pushl %edx; \
      movl $__KERNEL_DS, %edx;    \
      movl %edx, %ds;           \
      movl %edx, %es


#endif  /* __ENTRY_H__ */

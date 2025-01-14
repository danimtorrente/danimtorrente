#include <utils.h>
#include <types.h>

#include <mm_address.h>

/*
Copia de from hasta from + size del proceso src en el espacio logico de current() a
partir de la dirección to
*/
int copy_memory_from(struct task_struct* SRC, char* from, int size, char* to) {
	page_table_entry * pageTable = (page_table_entry *) get_DIR(SRC);
	int result = 0;
	int page = -1;
	for (int i = 0; i < TOTAL_PAGES && result < 2; i++) { //que no me salga de su size y algun booleano para salir dlebucle
		if (result == 1) {
			if (pageTable[i].bits.present == 0) {
				 ++result;
				page = i-1;
			}
			else result = 0; //comporbar si hay dos paginas consecutivas libres
		}
		else if (pageTable[i].bits.present == 0) ++result;
	}
	if (result == 2) { // dir + from hasta llegar a from +size
		for (int i = page; i < page+2; i++) {
			int frame = alloc_frame();
			set_ss_pag(pageTable, i, frame);
			set_cr3(pageTable);
			copy_data((void *)(from + PAGE_SIZE*(i-page)), (void *) pageTable[i].bits.pbase_addr,PAGE_SIZE);
		} return 0;
	}
	return -1; // si no hay dos paginas consecutivas libres retorno un error, ya que no se pide nada adicional
}

void copy_data(void *start, void *dest, int size)
{
  DWord *p = start, *q = dest;
  Byte *p1, *q1;
  while(size > 4) {
    *q++ = *p++;
    size -= 4;
  }
  p1=(Byte*)p;
  q1=(Byte*)q;
  while(size > 0) {
    *q1++ = *p1++;
    size --;
  }
}
/* Copia de espacio de usuario a espacio de kernel, devuelve 0 si ok y -1 si error*/
int copy_from_user(void *start, void *dest, int size)
{
  DWord *p = start, *q = dest;
  Byte *p1, *q1;
  while(size > 4) {
    *q++ = *p++;
    size -= 4;
  }
  p1=(Byte*)p;
  q1=(Byte*)q;
  while(size > 0) {
    *q1++ = *p1++;
    size --;
  }
  return 0;
}
/* Copia de espacio de kernel a espacio de usuario, devuelve 0 si ok y -1 si error*/
int copy_to_user(void *start, void *dest, int size)
{
  DWord *p = start, *q = dest;
  Byte *p1, *q1;
  while(size > 4) {
    *q++ = *p++;
    size -= 4;
  }
  p1=(Byte*)p;
  q1=(Byte*)q;
  while(size > 0) {
    *q1++ = *p1++;
    size --;
  }
  return 0;
}

/* access_ok: Checks if a user space pointer is valid
 * @type:  Type of access: %VERIFY_READ or %VERIFY_WRITE. Note that
 *         %VERIFY_WRITE is a superset of %VERIFY_READ: if it is safe
 *         to write to a block, it is always safe to read from it
 * @addr:  User space pointer to start of block to check
 * @size:  Size of block to check
 * Returns true (nonzero) if the memory block may be valid,
 *         false (zero) if it is definitely invalid
 */
int access_ok(int type, const void * addr, unsigned long size)
{
  unsigned long addr_ini, addr_fin;

  addr_ini=(((unsigned long)addr)>>12);
  addr_fin=((((unsigned long)addr)+size)>>12);
  if (addr_fin < addr_ini) return 0; //This looks like an overflow ... deny access

  switch(type)
  {
    case VERIFY_WRITE:
      /* Should suppose no support for automodifyable code */
      if ((addr_ini>=USER_FIRST_PAGE+NUM_PAG_CODE)&&
          (addr_fin<=USER_FIRST_PAGE+NUM_PAG_CODE+NUM_PAG_DATA))
	  return 1;
    default:
      if ((addr_ini>=USER_FIRST_PAGE)&&
  	(addr_fin<=(USER_FIRST_PAGE+NUM_PAG_CODE+NUM_PAG_DATA)))
          return 1;
  }
  return 0;
}


#define CYCLESPERTICK 109000

/*
 * do_div() is NOT a C function. It wants to return
 * two values (the quotient and the remainder), but
 * since that doesn't work very well in C, what it
 * does is:
 *
 * - modifies the 64-bit dividend _in_place_
 * - returns the 32-bit remainder
 *
 * This ends up being the most efficient "calling
 * convention" on x86.
 */
#define do_div(n,base) ({ \
        unsigned long __upper, __low, __high, __mod, __base; \
        __base = (base); \
        asm("":"=a" (__low), "=d" (__high):"A" (n)); \
        __upper = __high; \
        if (__high) { \
                __upper = __high % (__base); \
                __high = __high / (__base); \
        } \
        asm("divl %2":"=a" (__low), "=d" (__mod):"rm" (__base), "0" (__low), "1" (__upper)); \
        asm("":"=A" (n):"a" (__low),"d" (__high)); \
        __mod; \
})


#define rdtsc(low,high) \
        __asm__ __volatile__("rdtsc" : "=a" (low), "=d" (high))

unsigned long get_ticks(void) {
        unsigned long eax;
        unsigned long edx;
        unsigned long long ticks;

        rdtsc(eax,edx);

        ticks=((unsigned long long) edx << 32) + eax;
        do_div(ticks,CYCLESPERTICK);

        return ticks;
}

void memset(void *s, unsigned char c, int size)
{
  unsigned char *m=(unsigned char *)s;
  
  int i;
  
  for (i=0; i<size; i++)
  {
    m[i]=c;
  }
}

/*
 * interrupt.c -
 */
#include <types.h>
#include <interrupt.h>
#include <segment.h>

#include <hardware.h>
#include <io.h>
#include <misc.h>
#include <mm.h>

#include <devices.h>
#include <zeos_interrupt.h>

Gate idt[IDT_ENTRIES];
Register    idtR;
int zeos_ticks;

char char_map[] =
{
  '\0','\0','1','2','3','4','5','6',
  '7','8','9','0','\'','�','\0','\0',
  'q','w','e','r','t','y','u','i',
  'o','p','`','+','\0','\0','a','s',
  'd','f','g','h','j','k','l','�',
  '\0','�','\0','�','z','x','c','v',
  'b','n','m',',','.','-','\0','*',
  '\0','\0','\0','\0','\0','\0','\0','\0',
  '\0','\0','\0','\0','\0','\0','\0','7',
  '8','9','-','4','5','6','+','1',
  '2','3','0','\0','\0','\0','<','\0',
  '\0','\0','\0','\0','\0','\0','\0','\0',
  '\0','\0'
};

void keyboard_handler();
void system_call();
void clock_handler();
void page_handler(); //revisar si tiene parametros porque creo que tiene el del error que son 4 bytes


void setInterruptHandler(int vector, void (*handler)(), int maxAccessibleFromPL)
{
  /***********************************************************************/
  /* THE INTERRUPTION GATE FLAGS:                          R1: pg. 5-11  */
  /* ***************************                                         */
  /* flags = x xx 0x110 000 ?????                                        */
  /*         |  |  |                                                     */
  /*         |  |   \ D = Size of gate: 1 = 32 bits; 0 = 16 bits         */
  /*         |   \ DPL = Num. higher PL from which it is accessible      */
  /*          \ P = Segment Present bit                                  */
  /***********************************************************************/
  Word flags = (Word)(maxAccessibleFromPL << 13);
  flags |= 0x8E00;    /* P = 1, D = 1, Type = 1110 (Interrupt Gate) */

  idt[vector].lowOffset       = lowWord((DWord)handler);
  idt[vector].segmentSelector = __KERNEL_CS;
  idt[vector].flags           = flags;
  idt[vector].highOffset      = highWord((DWord)handler);
}

void setTrapHandler(int vector, void (*handler)(), int maxAccessibleFromPL)
{
  /***********************************************************************/
  /* THE TRAP GATE FLAGS:                                  R1: pg. 5-11  */
  /* ********************                                                */
  /* flags = x xx 0x111 000 ?????                                        */
  /*         |  |  |                                                     */
  /*         |  |   \ D = Size of gate: 1 = 32 bits; 0 = 16 bits         */
  /*         |   \ DPL = Num. higher PL from which it is accessible      */
  /*          \ P = Segment Present bit                                  */
  /***********************************************************************/
  Word flags = (Word)(maxAccessibleFromPL << 13);

  //flags |= 0x8F00;    /* P = 1, D = 1, Type = 1111 (Trap Gate) */
  /* Changed to 0x8e00 to convert it to an 'interrupt gate' and so
     the system calls will be thread-safe. */
  flags |= 0x8E00;    /* P = 1, D = 1, Type = 1110 (Interrupt Gate) */

  idt[vector].lowOffset       = lowWord((DWord)handler);
  idt[vector].segmentSelector = __KERNEL_CS;
  idt[vector].flags           = flags;
  idt[vector].highOffset      = highWord((DWord)handler);
}


void setIdt()
{
  /* Program interrups/exception service routines */
  idtR.base  = (DWord)idt;
  idtR.limit = IDT_ENTRIES * sizeof(Gate) - 1;

  set_handlers();

  setInterruptHandler(33, keyboard_handler, 0); // verificar parametros
 // setInterruptHandler(0x80, system_call, 0); // verificar parametros
  setInterruptHandler(32, clock_handler, 0);
  setTrapHandler(0x80, system_call, 3);
  setInterruptHandler(14, page_handler, 0);
//  setTrapHandler(14, page_handler, 0);
  set_idt_reg(&idtR);
}


void keyboard_routine() {
	int port = 0x60;
	unsigned char input = inb(port);
	unsigned char mb = input >> 7;
	if (!mb) {
		if (char_map[input & 0x7F] == '\0') printc_xy(0,0,'C');
		else printc_xy(0,0,char_map[input & 0x7F]); // printc_xy(Byte mx, Byte my, char c)
	}
}

void schedule() {
    update_sched_data_rr();

    if (needs_sched_rr()) {
        update_process_state_rr(current(), &readyqueue);
        sched_next_rr();
    }
}

void clock_routine() {
	zeos_ticks += 1;
	zeos_show_clock();
//////////////////////////////////////////////////////
	schedule();
}

void page_routine(int error, int address) {

	char mess[] = "Process generates a PAGE FAULT exception at EIP: 0x";
	int len = strlen(mess);
	sys_write_console(mess, len);
	len = 0;
	char num[30];
	itoa(address, num);
	len = strlen(num);
	sys_write_console(num, len);

//	printk(mess);
//	printk(num);

	while(1);
}

///////////////////////////////////////////////////////////////////////////////////////

/* evec.c -- initevec, doevec */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <tty.h>
#include <q.h>
#include <io.h>
#include <sunromvec.h>
#include <cpu.addrs.h>    
#include <interreg.h>    

#define NUMEVEC 256    

/* the Xinu exception vector */    
long  evec[NUMEVEC];

char *origevec;

int trapX();

initevec()
{
    int e;
    long *new,*old;

    /* copy the original */
    new = evec;
    old = (long *) 0xfe60c00;

    for (e=0; e<NUMEVEC; ++e)
	*new++ = *old++;

    /* save d7 */
    asm("movl	d7, sp@-");
    
    /* swap in the new one*/
    asm("movc	vbr,d7");
    asm("movl	d7,_origevec");
    
    asm("movl	#_evec,d7");
    asm("movc	d7,vbr");

    /* restore d7 */
    asm("movl	sp@+,d7");

    /* turn off the level 7 clock */
    *INTERRUPT_BASE &= ~(IR_ENA_CLK7 | IR_ENA_CLK5);  /* Unhang flops */

    for (e=2; e<NUMEVEC; ++e)
        set_evec(e*4,trapX);
    
}


doevec(arg)
     int arg;
{
    unsigned short sr;
    unsigned long pc;
    unsigned short vector;
    int evnum;
    char *parg;

    parg = (char *) &arg;

    sr = *((short *) parg);
    pc = *((long  *) (parg+2));
    vector = (*((short *) (parg+6))) & 0x0fff;    /* only low 12 bits */
    
    kprintf("\nXinu Trap!  ");
    kprintf("sr: 0x%x  ", sr);
    kprintf("exception: 0x%x  ", vector);
    kprintf("pc: 0x%lx\n",pc);
    kprintf("Trap definition: ");

    evnum = vector/4;
    if (evnum<32)
	switch(evnum) {
	    case  0: kprintf("RESET0, SHOULDN'T SEE THIS\n"); break;
	    case  1: kprintf("RESET1, SHOULDN'T SEE THIS\n"); break;
	    case  2: kprintf("Bus Error\n"); break;
	    case  3: kprintf("Address Error\n"); break;
	    case  4: kprintf("Illegal instruction\n"); break;
	    case  5: kprintf("Divide by 0\n"); break;
	    case  6: kprintf("CHK,CHK2 software trap\n"); break;
	    case  7: kprintf("cpTRAPcc, TRAPcc, TRAPV software trap\n"); break;
	    case  8: kprintf("Privilege Violation\n"); break;
	    case  9: kprintf("Trace\n"); break;
	    case 10: kprintf("Line 1010 Emulator, illegal instruction\n"); break;
	    case 11: kprintf("Line 1111 Emulator, illegal instruction\n"); break;

	    case 13: kprintf("Coprocessor protocol violation\n"); break;
	    case 14: kprintf("Format Error\n"); break;
	    case 15: kprintf("Uninitialized Interrupt\n"); break;

	    case 24: kprintf("Spurrious Interrupt\n"); break;
	    case 25: kprintf("Level 1 Auto Vector\n"); break;
	    case 26: kprintf("Level 2 Auto Vector\n"); break;
	    case 27: kprintf("Level 3 Auto Vector\n"); break;
	    case 28: kprintf("Level 4 Auto Vector\n"); break;
	    case 29: kprintf("Level 5 Auto Vector (clock)\n"); break;
	    case 30: kprintf("Level 6 Auto Vector (serial line)\n"); break;
	    case 31: kprintf("Level 7 Auto Vector (monitor clock)\n"); break;
	    default: kprintf("Unassigned (reserved)\n"); break;
	  }
    else if (evnum<48)
	kprintf("Trap #%d instruction trap\n",(evnum-32));
    else if (evnum<54)
	kprintf("FPCP trap\n");
    else if (evnum<59)
	kprintf("PMMU trap\n");
    else if (evnum<64)
	kprintf("Unassigned, reserved\n");
    else
	kprintf("User defined vector\n");

    kprintf("\n");
    evecnocont();
}

/* We want to keep people from continuing when an exception occurs */
evecnocont()
{
    while (TRUE) {
	ret_mon();
	kprintf("Can't continue from exception\n");
    }

}


/* ttyutils.c */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
#include <slu.h>
#include <zsreg.h>

int fttyoint = FALSE;    

PrintRegs(addr)
     char *addr;
{
    int i;
    int bits;
    for (i=0; i<= 3; ++i) {
	*addr = i;
	DELAY(3);
	bits = *addr;
	DELAY(3);
	kprintf("R%d: %08b ",i,(unsigned int) (bits & 0x00ff) );
    }
    kprintf("\n");
}


/*
 * ttyostop -- turn off tty xmit interrupts
 */
ttyostop(iptr)
    register struct tty *iptr;
{
    STATWORD ps;    
    struct zscc_device *zptr;
    
    disable(ps);

    DELAY(2);
    (iptr->ioaddr)->zscc_control = 1;
    DELAY(2);
    (iptr->ioaddr)->zscc_control = ZSWR1_RIE;
    fttyoint = FALSE;
    restore(ps);
}


/*
 * ttyostart -- turn on xmit interrupts
 */
ttyostart(iptr)
     struct tty *iptr;
{
    STATWORD ps;    
    disable(ps);
    if (fttyoint==FALSE) {
	DELAY(2);
	(iptr->ioaddr)->zscc_control = 1;
	DELAY(2);
	(iptr->ioaddr)->zscc_control = ZSWR1_RIE | ZSWR1_TIE;
	fttyoint = TRUE;
	ttyoin(iptr);
    }
    restore(ps);
}

/*
 * printareg(reg)
 */
PrintAReg(reg)
     unsigned long reg;
{
    kprintf("Reg: %08lb\n",reg);
}








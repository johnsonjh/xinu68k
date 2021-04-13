/*------------------------------------------------------------------------
 *	ret_to_monitor -- return to the Sun monitor
 *------------------------------------------------------------------------
 */

#include <conf.h>
#include <kernel.h>
#include <cpu.addrs.h>    
#include <interreg.h>
#include <sunromvec.h>    
#include <clock.h>
#include <proc.h>    

/*
 *   Return to the Sun monitor program.  Great pains are taken so that you can use
 *   the <C>ontinue command from the monitor to return to the program.  I is the   
 *   responsibility of the procedure calling ret_mon() to insure that it is
 *   re-entrant at that point.
 *
 *   Currently called by:
 *     ttyiin -- on BREAK
 *     doevec -- when exception occurs
 *     xdone  -- when all user processes have finished
 */

ret_mon()
{
    unsigned char junk;

    /* we use D7, so we'd better save it here */
    asm("movl	d7,sp@-");

    /* put the existing registers into the current process's proc entry */
    ctxsw(proctab[currpid].pregs,proctab[currpid].pregs);

    /* turn off ALL clock interrupts */
    *INTERRUPT_BASE &= ~IR_ENA_INT;
    *INTERRUPT_BASE &= ~(IR_ENA_CLK7 | IR_ENA_CLK5);
    
    /* return to the monitor interrupt vector */
    asm("movl	_origevec,d7");
    asm("movc	d7,vbr");

    /* reset the clock stuff */
    *INTERRUPT_BASE &= ~IR_ENA_INT;
    *INTERRUPT_BASE &= ~(IR_ENA_CLK7 | IR_ENA_CLK5);	/* clear ALL interrupts */
    CLOCK_BASE->clk_intrreg = CLK_INT_HSEC;	/* set back to 1/100 second */
    CLOCK_BASE->clk_cmd = (CLK_CMD_NORMAL & ~CLK_CMD_INTRENA);
    junk = CLOCK_BASE->clk_intrreg;		/* clear clock */
    *INTERRUPT_BASE |=  IR_ENA_CLK7;		/* allow level 7 interrupts */
    CLOCK_BASE->clk_cmd = CLK_CMD_NORMAL;	/* enable clock intr */
    *INTERRUPT_BASE |= IR_ENA_INT;		/* enable interrupts */
    
    /* turn off the led's */
    (romp->v_set_leds)((char) 0xff);

    /*
     *  Now, jump to the monitor, and MAYBE return
     */
    asm("trap #14");

    /* turn off ALL clock interrupts */
    *INTERRUPT_BASE &= ~IR_ENA_INT;
    *INTERRUPT_BASE &= ~(IR_ENA_CLK7 | IR_ENA_CLK5);
    
    /* get our exception vector back*/
    asm("movl	#_evec,d7");
    asm("movc	d7,vbr");

    /* reset the clock stuff */
    *INTERRUPT_BASE &= ~IR_ENA_INT;
    *INTERRUPT_BASE &= ~(IR_ENA_CLK7 | IR_ENA_CLK5);	/* clear ALL interrupts */
    CLOCK_BASE->clk_intrreg = CLK_INT_TSEC;	/* set back to 1/10 second */
    CLOCK_BASE->clk_cmd = (CLK_CMD_NORMAL & ~CLK_CMD_INTRENA);
    junk = CLOCK_BASE->clk_intrreg;		/* clear clock */
    *INTERRUPT_BASE |=  IR_ENA_CLK7;		/* allow level 7 interrupts */
    CLOCK_BASE->clk_cmd = CLK_CMD_NORMAL;	/* enable clock intr */
    *INTERRUPT_BASE |= IR_ENA_INT;		/* enable interrupts */

    /* get the original d7 back */
    asm("movl	sp@+,d7");
}



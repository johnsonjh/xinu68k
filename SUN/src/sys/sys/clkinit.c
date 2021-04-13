/* clkinit.c - clkinit, updateleds, dog_timeout */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <cpu.addrs.h>    
#include <clock.h>
#include <interreg.h>
#include <sunromvec.h>
    
/* real-time clock variables and sleeping process queue pointers	*/
    
#ifdef	RTCLOCK
int	count6;			/* counts in 60ths of a second 6-0	*/
int	count10;		/* counts in 10ths of second 10-0	*/
int	clmutex;		/* mutual exclusion for time-of-day	*/
long	clktime;		/* current time in seconds since 1/1/70	*/
int     defclk;			/* non-zero, then deferring clock count */
int     clkdiff;		/* deferred clock ticks			*/
int     slnempty;		/* FALSE if the sleep queue is empty	*/
int     *sltop;			/* address of key part of top entry in	*/
				/* the sleep queue if slnonempty==TRUE	*/
int     clockq;			/* head of queue of sleeping processes  */
int	preempt;		/* preemption counter.	Current process */
				/* is preempted when it reaches zero;	*/
#ifdef	RTCLOCK
				/* set in resched; counts in ticks	*/
int	clkruns;		/* set TRUE iff clock exists by setclkr	*/
#else
int	clkruns = FALSE;	/* no clock configured; be sure sleep	*/
#endif				/*   doesn't wait forever		*/

/*
 *------------------------------------------------------------------------
 * clkinit - initialize the clock and sleep queue (called at startup)
 *------------------------------------------------------------------------
 */
clkinit()
{
	int junk;
	WORD *vector;

	int Asm_clkint();

	set_evec((int) C5VECTOR, Asm_clkint);
	set_evec((int) C7VECTOR, Asm_clkint);

	/* setclkr(); */
	preempt = QUANTUM;		/* initial time quantum		*/
	count6 = 6;			/* 60ths of a sec. counter	*/
	count10 = 10;			/* 10ths of a sec. counter	*/
	clmutex = screate(1);		/* semaphore for tod clock	*/
	clktime = 0L;			/* initially a low number	*/
	slnempty = FALSE;		/* initially, no process asleep	*/
	clkdiff = 0;			/* zero deferred ticks		*/
	defclk = 0;			/* clock is not deferred	*/
	clkruns = 1;
	clockq = newqueue();
	
	/*
	 * This hardware is sufficiently delicate that we need to follow
	 * this order:
	 *	Reset interrupt-catching flops
	 *	Clear and disable TOD chip interrupts
	 *	Enable interrupt-catching flop(s)
	 *	Enable TOD chip interrupts
	 */
	CLOCK_BASE->clk_cmd = CLK_CMD_NORMAL; 
	*INTERRUPT_BASE &= ~(IR_ENA_CLK7 | IR_ENA_CLK5);  /* Unhang flops */
        CLOCK_BASE->clk_intrreg = 0;			 /* Disable TOD ints */
	junk = CLOCK_BASE->clk_intrreg;			/* Maybe clr pending int */
        *INTERRUPT_BASE |=  IR_ENA_CLK7;		/* Prime the flops */
	CLOCK_BASE->clk_intrreg = CLK_INT_TSEC;		/* allow TOD ints every 1/10 sec*/
}
#endif


#define set_leds(x) (romp->v_set_leds)((char) x)

/* set the pattern to use in blinking the LEDs */
char ledpattern[] = {
    0x00,	/* ---- ---- */
    0x18,	/* ---X X--- */
    0x3c,	/* --XX XX-- */
    0x7e,	/* -XXX XXX- */
    0xff,	/* XXXX XXXX */
    0xe7,	/* XXX- -XXX */
    0xc3,	/* XX-- --XX */
    0x81,	/* X--- ---X */
    0x00,	/* ---- ---- */
    0x81,	/* X--- ---X */
    0xc3,	/* XX-- --XX */
    0xe7,	/* XXX- -XXX */
    0xff,	/* XXXX XXXX */
    0x7e,	/* -XXX XXX- */
    0x3c,	/* --XX XX-- */
    0x18	/* ---X X--- */
    };

/*
 * updateleds -- update the status leds in the above pattern
 */
int pled = 0;
updateleds()
{
	/* play with the led's */
	pled = (pled+1) % sizeof(ledpattern);
	
	set_leds(~(ledpattern[pled])); 
}


/*
 * dog_timeout -- called when the watchdog timer determines that
 * interrupts have been disabled for too long
 */
dog_timeout()
{
    STATWORD ps;

    disable(ps);
    kprintf("\n\nWatchdog timeout!! -- interrupts disabled too long.\n");
    ret_mon();
    restore(ps);
    return;
}

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#ifdef ATT7300
#include <intvectors_7300.h>
#include <mcr7300.h>
#else
#include <intvectors_mecb.h>
#include <clock.h>
#endif

extern clkint(),spurint();
extern count6,preempt ;
#ifdef ATT7300
/* ATT7300 misc. control reg. is write-only, so keep copy of its normal
   value in memory--currently only used for clock tick acknowledge */
unsigned short soft_mcr7300 = 0;
unsigned short *tstat ; /* global pointer to timer tstat, used in clkint.s */
#else
unsigned char *tstat ; /* global pointer to timer tstat, used in clkint.s */
#endif
/*
 *------------------------------------------------------------------
 * clkinit - initialize the clock and sleep queue (called at startup)
 *------------------------------------------------------------------
 */
clkinit()
{
  int len;
  int clockqtail;		/* need place to put it, but never used */

  CLOCKVEC = clkint;		/* set up interrupt vector */
  setclkr();			/* set  global flag clock on */
  preempt = QUANTUM;		/* initial time quantum		*/
  count6 = 6;			/* 60ths of a sec. counter	*/
  slnempty = FALSE;		/* initially, no process asleep */
  clkdiff = 0;			/* zero deferred ticks          */
  defclk = 0;			/* clock is not deferred        */
  newqueue(&clockq,&clockqtail);/* allocate clock queue in q    */

#ifdef ATT7300
  tstat = MCR7300;		/* pointer to MCR for clkint.s */
  soft_mcr7300 |= CLRCLKINT;	/* bit was low from sys reset, make hi */
  kprintf("starting clock...\n");
  *MCR7300 = soft_mcr7300;	/* put new val in hardware MCR */
#else
  /* Added for MECB: set up timer of 68230 as line clock--      */    
  /* specify clock int location to clock device, as int vect slot#-- */
  SPURVEC = spurint;            /* and handle spurious ints too */ 
  TIMER->tivr = (char)(((int)CLOCKVECADDRESS)>>2); /* i.e div by 4 */
  /* specify downcount to be loaded at each zero detect--       */
  /* 3 byte register at odd-addressed bytes, 1 downcount=8usec  */
  TIMER->cpr_high = 0;
  TIMER->cpr_med = 8;		/* approx 16 msecs apart (8*256*8=16K usec) */
  TIMER->cpr_low = 0;		/* --i.e, approx 1/60 sec, like PDP11 clock */
  kprintf("starting clock...\n");
  TIMER->tcsr = (CLOCKIE|LOADFROMCPR|PRESCALE32|CLOCKON); /* start timer */
  tstat = &TIMER->tstat; /* set up the global address of timer tstat */
  TIMER->tstat = 1;   /* reset counter (otherwise wait to roll over, ~2min) */
#endif
}

spurintc()
{
	kprintf("Received a spurious interrupt\n");
}

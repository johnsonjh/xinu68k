/* Standalone program exercising the timer device on the mecb */
/* Vals for PLATFORM (have to agree with Makefile)--          */
#define MECB 1
#define ATT7300 2
#if PLATFORM == MECB
#include <intvectors_mecb.h>
#include <clock68230.h>
#elif PLATFORM == ATT7300
#include <intvectors_7300.h>
#include <mcr7300.h>
#endif
#include <stdio.h>

/* these as routines are in mecb library--see comment in intvectors_mecb.h  */
/* they just save C scratch regs, call _<routine>c, and restore */
/* the same regs saved before */
extern clockint(),spurint();

main()
{
  int len;

#if PLATFORM == MECB
  /* specify clock int location to clock device, as int vect slot#-- */
  TIMER->tivr = (char)(((int)CLOCKVECADDRESS)>>2); /* i.e div by 4 */
  CLOCKVEC = clockint;		/* set up to handle such interrupts */
  SPURVEC = spurint;		/* and handle spurious ints too */
  TIMER->cpr_high = 2;		/* for approx 1 sec ints */
  TIMER->cpr_med = 0;
  TIMER->cpr_low = 0;
  printf("starting clock...");
  TIMER->tcsr = (CLOCKIE|LOADFROMCPR|PRESCALE32|CLOCKON); /* start timer */
  TIMER->tstat = 1;   /* reset counter (otherwise wait to roll over, ~2min) */
#elif PLATFORM == ATT7300
  CLOCKVEC = clockint;		/* set up to handle such interrupts */
  printf("starting clock...");
  *MCR7300 = CLRCLKINT;	/* bit was low from sys reset, make hi */
#else
  printf("timetest is implemented only for MECB and ATT7300\n");
#endif
  setlowpri();        /* all ready--start normal priority (0) processing */
  kprintf("...OK\n");		/* use kprintf after setlowpri... */
  while (1);
}
#if PLATFORM == ATT7300
static int count;
#endif

/* C interrupt handler, called from _clockint, the as "shell" routine */
/* _clockint saves and restores the C scratch registers */
clockintc()
{
#if PLATFORM == MECB
  TIMER->tstat = 1;		/* tell clock device we saw tick */
  kprintf("%c",'.');		/* not normal int handler behavior-- */
#elif PLATFORM == ATT7300
  *MCR7300 = 0;		/* toggle from high to low to high */
  *MCR7300 = CLRCLKINT;
  count++;			/* confuse optimizer from removing =0 */
  if (count%60==0)		/* 60/sec--print a dot ea sec */
    kprintf("%c",'.');		/* not normal int handler behavior-- */
#endif
}

spurintc()
{
  kprintf("got spurious int!!"); /* just in case something is set up wrong, */
}				/*   or the board int. logic is broken */

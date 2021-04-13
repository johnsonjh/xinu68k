/* Standalone program exercising the timer device on the mecb */

#include <intvectors_mecb.h>
#include <clock68230.h>
#include <stdio.h>

/* these as routines are in mecb library--see comment in intvectors_mecb.h  */
/* they just save C scratch regs, call _<routine>c, and restore */
/* the same regs saved before */
extern clockint(),spurint();

main()
{
  int len;

  /* specify clock int location to clock device, as int vect slot#-- */
  TIMER->tivr = (char)(((int)CLOCKVECADDRESS)>>2); /* i.e div by 4 */
  CLOCKVEC = clockint;		/* set up to handle such interrupts */
  SPURVEC = spurint;		/* and handle spurious ints too */
  printf("Enter len of ticks in secs: ");
  scanf("%d",&len);
  TIMER->cpr_high = 2*len;	/* for approx 1 sec ints */
  TIMER->cpr_med = 0;
  TIMER->cpr_low = 0;
  printf("starting clock...");
  TIMER->tcsr = (CLOCKIE|LOADFROMCPR|PRESCALE32|CLOCKON); /* start timer */
  TIMER->tstat = 1;   /* reset counter (otherwise wait to roll over, ~2min) */
  setlowpri();        /* all ready--start normal priority (0) processing */
  kprintf("...OK\n");		/* use kprintf after setlowpri... */
  while (1);
}

/* C interrupt handler, called from _clockint, the as "shell" routine */
/* _clockint saves and restores the C scratch registers */
clockintc()
{
  TIMER->tstat = 1;		/* tell clock device we saw tick */
  kprintf("%c",'.');		/* not normal int handler behavior-- */
}				/*  shouldn't poll at int level! */

spurintc()
{
  kprintf("got spurious int!!"); /* just in case something is set up wrong, */
}				/*   or the board int. logic is broken */

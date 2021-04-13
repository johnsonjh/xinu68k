/* Standalone program exercising the timer device on the ATT7300 */

#include <stdio.h>
#include <intvectors_7300.h>
#include <mcr7300.h>

/* these as routines, clockint, etc., are in the sa library  */
/* they just save C scratch regs, call _<routine>c, and restore */
/* the same regs saved before */
extern clockint(),spurint();

main()
{
  CLOCKVEC = clockint;		/* set up to handle such interrupts */
  printf("starting clock...");
  *MCR7300 = CLRCLKINT;	/* bit was low from sys reset, make hi */
  setlowpri();        /* all ready--start normal priority (0) processing */
  kprintf("...OK\n");		/* use kprintf after setlowpri */
  while (1);
}

/* C interrupt handler, called from _clockint, the as "shell" routine */
/* _clockint saves and restores the C scratch registers */
clockintc()
{
  static int count = 0;

  *MCR7300 = 0;		/* toggle from high to low to high */
  *MCR7300 = CLRCLKINT;
  count++;
  if (count%60==0)		/* 60/sec--print a dot ea sec */
    kprintf("%c",'.');		/* not normal int handler behavior-- */
}				/*  shouldn't poll at int level! */

spurintc()
{
  kprintf("got spurious int!!"); /* just in case something is set up wrong, */
}				/*   or the board int. logic is broken */

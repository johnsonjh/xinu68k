/* Since the console keyboard and output device share the same int vect */
/*   we name the common int handler envelope routine "slu1int  */
/*   (previously named "kbint")  */
/* The needed interrupt envelope routines slu1int, etc. are now in  */
/*   the "C" library, and setlowpri is there too.  */
/*   See intvectors.h for more info.                             */

#include <stdio.h>
#include <intvectors_7300.h>
#include <slu7201.h>
/* CONSOLE SLU memory-mapped i/o register address-- */
#define SLUCSR1 ((volatile struct csr *)0xe50000)

/* the following is needed so that C knows that "slu1int" is a fn */
/*   --note that C cannot figure that out from "= slu1int" below */
extern slu1int();		/* the as68 shell routine for slu1intc */
/* the following is not required--C can figure out that setlowpri  */
/* must be a fn from the call "setlowpri()" below */
extern setlopri();		/* as routine to set 0 CPU priority */

main()
{
				/* arm keyboard interrupt-- */
  SLUVEC1 = slu1int;		/* as routine which calls slu1intc */

   /* use set_control_reg rather than SLUCSR1->cstat = ... so that */
   /* you can use kprintf for debugging--it needs to know how you */
   /* set the control reg, so it can put it back that way */
 /* default+recv int ena'd--control reg 1 in 7201-- */	
  kprintf("Starting...\n");
  set_control_reg7201(SLUCSR1,SLUCREG1DEF|SLURECVIE,1);
  kprintf("Going low pri...\n");
  /*  now hardware reg's are all set up, time to step down to normal pri-- */
  setlowpri();			/* run at low CPU pri "process level"*/

  while (1);			/*loop forever...*/
}


slu1intc()
{
  char ch;
  struct csr *csrptr = (struct csr *)SLUCSR1;

  ch = csrptr->cbuf;		/*read char*/
  kprintf("ch=%c ",ch);		/* prove kprintf works */
  /* NOTE:  normally we never wait in an int routine--why? */
  while((csrptr->cstat & SLUTRANSREADY) == 0) ; /* busy loop */
  csrptr->cbuf = ch;		/* echo char*/
}

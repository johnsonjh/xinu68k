/* kprintf.c */

/* Here, SLU is short for "serial line unit", that is, the serial */
/* interface chip, a 7201A dual UART on the AT&T 7300, a MC6850
   on the MECB--also called a UART */

/* kprintf is the routine to use for debugging output when the calling */
/* program also uses console interrupts.  It shuts out interrupts */
/* by going high-CPU-priority, saving the old CPU priority locally and */
/* reestablishing it on return */

#include <stdio.h>
#ifdef ATT7300
#include "sludevs_7300.h"
#include "slu7201.h"
#else
#include "sludevs_mecb.h"
#include "slu6850.h"
#endif

/* as in Xinu's "kernel.h"-- */
#define OK 1
static int savesr;

/*------------------------------------------------------------------------
 *  kprintf  --  kernel printf: formatted, unbuffered output to console 
 *------------------------------------------------------------------------
 */
kprintf(fmt, args)		/* Derived by Bob Brown, Purdue U.	*/
        char *fmt;
{
  static unsigned short savesr;
  extern int kputc7201(),kputc6850();
  extern int _disable(),_restore();

  _disable(&savesr);		/* use own savesr--"disable" has another */

#ifdef ATT7300
  _doprnt(fmt, &args, kputc7201, SLUCSR1); /* console is 7201 */
#else
  _doprnt(fmt, &args, kputc6850, SLUCSR1); /* console is 6850 */
#endif

  _restore(&savesr);
  return(OK);
}


/* kprintf.c - kprintf, kputc--slightly edited from original Xinu */

#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <tty.h>

/* set up macros for appropriate SLU as spec. by SLUCHIP:
   if SLUCHIP==6850, #include slu6850.h, #define TTYWRITE ttywrite6850, etc.
   if SLUCHIP==7201, #include slu7201.h, #define TTYWRITE ttywrite7201, etc.
   if SLUCHIP undef.,#include slu.h, #define TTYWRITE ttywrite  (orig. case)
   similarly sluaccess.h, other TTY fns */
#include <sluinclude.h>
#ifdef ATT7300
int	slowtime = 500;
/* Xinu console is the serial port on the back, assumed already
   initialized by the downloading support */
#else
int     slowtime = 200;
/* MECB console is the console MC6850, init'd by TUTOR */
#endif

/*------------------------------------------------------------------------
 *  kprintf  --  kernel printf: formatted, unbuffered output to CONSOLE
 *------------------------------------------------------------------------
 */

/* kprintf is the routine to use for polling output, even in the case that */
/* the calling program also uses SLU interrupts.  It shuts out interrupts */
/* by going high-CPU-priority, saving the old CPU priority locally and */
/* reestablishing it on return.  It varies from the original Vol I. Xinu */
/* sources by not changing the device control register, but rather just */
/* relying on high CPU priority to hold the console (did the original SLU */
/* fail to post a transmit ready bit with interrupts enabled??) */

kprintf(fmt, args)		/* Derived by Bob Brown, Purdue U.	*/
        char *fmt;
{
  static unsigned short savesr;
  int     kputc();

  _disable(&savesr);		/* orig. Xinu had "savestate()" */
  _doprnt(fmt, &args, kputc, CONSOLE);
  _restore(&savesr);		/* orig. Xinu had "rststate()" */
  return(OK);
}

/*------------------------------------------------------------------------
 *  kputc  --  write a character on the console (or other SLU)
 *  using polled I/O
 *------------------------------------------------------------------------
 */
kputc(device,c)
int device;
register char c;	/* character to print from _doprnt	*/
{
        struct csr *csrptr;
/*T*/	int	slowdown;	/* added to delay output because host	*/
/*T*/				/* can't take it at 9600 baud		*/
	if ( c == 0 )
		return;
	if ( c == NEWLINE )
		kputc(device, RETURN );
	csrptr = (struct csr *)devtab[device].dvcsr;
	while ( (csrptr->cstat & SLUTRANSREADY) == 0 ) ;  /* poll for idle*/
	csrptr->cbuf = c;
/*T*/	for(slowdown=0;slowdown<slowtime;slowdown++) ;
	while ( (csrptr->cstat & SLUTRANSREADY) == 0 ) ;  /* poll for idle*/
}

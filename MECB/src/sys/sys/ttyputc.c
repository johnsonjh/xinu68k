/* ttyputc.c - ttyputc */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <tty.h>
#include <io.h>

/* set up macros for appropriate SLU as spec. by SLUCHIP:
   if SLUCHIP==6850, #include slu6850.h, #define TTYPUTC ttyputc6850, etc.
   if SLUCHIP==7201, #include slu7201.h, #define TTYPUTC ttyputc7201, etc.
   if SLUCHIP undef.,#include slu.h, #define TTYPUTC ttyputc  (orig. case)
   similarly sluaccess.h, other TTY fns */
#include <sluinclude.h>

/*------------------------------------------------------------------------
 *  ttyputc - write one character to a tty device
 *------------------------------------------------------------------------
 */
TTYPUTC(devptr,ch)
	struct	devsw	*devptr;
	char	ch;
{
	register struct	tty	*ttyp;
	register struct	csr	*cptr;

#ifdef	DEBUG
	dotrace("ttyputc", &devptr, 2);
#endif
	ttyp = &tty[devptr->dvminor];

	cptr = (struct csr *)devptr->dvcsr;
        if ( ch==NEWLINE && ttyp->ocrlf )
	  TTYPUTC(devptr,RETURN);
	wait(ttyp->osem);			/* wait	for space in queue*/
	disable();
	ttyp->obuff[ttyp->ohead++] = ch;
	if (ttyp->ohead	>= OBUFLEN)
		ttyp->ohead = 0;

	slutenable(cptr); /* enable transmitter ints if disabled */
	restore();
	return(OK);
}

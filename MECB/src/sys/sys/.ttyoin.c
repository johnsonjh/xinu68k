/* ttyoin.c - ttyoin */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>

#ifdef	lsi11
#	include <slu.h>
#else
#	include <necuart.h>
#endif

/*------------------------------------------------------------------------
 *  ttyoin  --  lower-half tty device driver for output interrupts
 *------------------------------------------------------------------------
 */
INTPROC	ttyoin(ttyp, cptr)
	struct	tty	*ttyp;
	struct	csr	*cptr;
{
	int	ct;

#ifdef	DEBUG
	dotrace("ttyoin", &ttyp, 2);
#endif
	if (ttyp->ehead	!= ttyp->etail)	{
		sluputch(cptr, ttyp->ebuff[ttyp->etail++]);
		if (ttyp->etail	>= EBUFLEN)
			ttyp->etail = 0;
		return;
	}
	if (ttyp->oheld) {			/* honor flow control	*/
		slutdisable(cptr, ttyp);
		return;
	}
	if ((ct=scount(ttyp->osem)) < OBUFLEN) {
		sluputch(cptr, ttyp->obuff[ttyp->otail++]);
		if (ttyp->otail	>= OBUFLEN)
			ttyp->otail = 0;
		if (ct > OBMINSP)
			signal(ttyp->osem);
		else if	( ++(ttyp->odsend) == OBMINSP) {
			ttyp->odsend = 0;
			signaln(ttyp->osem, OBMINSP);
		}
	} else
		slutdisable(cptr, ttyp);
}

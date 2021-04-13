/* ttyoin.c - ttyoin */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>

/* set up macros for appropriate SLU as spec. by SLUCHIP:
   if SLUCHIP==6850, #include slu6850.h, #define TTYOIN ttyoin6850, etc.
   if SLUCHIP==7201, #include slu7201.h, #define TTYOIN ttyoin7201, etc.
   if SLUCHIP undef.,#include slu.h, #define TTYOIN ttyoin  (orig. case)
   similarly sluaccess.h, other TTY fns */
#include <sluinclude.h>

/*------------------------------------------------------------------------
 *  ttyoin  --  output character to uart
 *------------------------------------------------------------------------
 */
TTYOIN(ttyp, cptr)
	register struct	tty	*ttyp;
	register struct	csr	*cptr;
{
	int	count;

	if (ttyp->ehead	!= ttyp->etail)	{
		sluputch(cptr, ttyp->ebuff[ttyp->etail++]);
		if (ttyp->etail	>= EBUFLEN)
			ttyp->etail = 0;
		return;
	}
	if (ttyp->oheld) {	/* honor flow control	*/
		slutdisable(cptr); /* disable transmitter ints if on */
		return;
	}
	if ((count=scount(ttyp->osem)) < OBUFLEN) {
		sluputch(cptr, ttyp->obuff[ttyp->otail++]);
		if (ttyp->otail >= OBUFLEN)
			ttyp->otail = 0;
		if (count > OBMINSP)
			signal(ttyp->osem);
		else if ( ++(ttyp->odsend) == OBMINSP) {
			ttyp->odsend = 0;
			signaln(ttyp->osem, OBMINSP);
		}
	} else
	        slutdisable(cptr);
}

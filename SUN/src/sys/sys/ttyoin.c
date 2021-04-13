/* ttyoin.c - ttyoin */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
#include <slu.h>
#include <zsreg.h>

/*------------------------------------------------------------------------
 *  ttyoin  --  lower-half tty device driver for output interrupts
 *------------------------------------------------------------------------
 */
INTPROC	ttyoin(iptr)
	register struct	tty   *iptr;
{
	STATWORD ps;    
	register struct	zscc_device	*zptr;
	int	ct;

	zptr = iptr->ioaddr;
	
        disable(ps);	

	if (iptr->ehead	!= iptr->etail)	{
		zptr->zscc_data = iptr->ebuff[iptr->etail++];
		if (iptr->etail	>= EBUFLEN)
			iptr->etail = 0;
		restore(ps);
		return;
	}
	if (iptr->oheld) {			/* honor flow control	*/
		ttyostop(iptr);
		restore(ps);
		return;
	}
	if ((ct=iptr->ocnt) > 0) {
		zptr->zscc_data = iptr->obuff[iptr->otail++];
	        --iptr->ocnt;
		if (iptr->otail	>= OBUFLEN)
			iptr->otail = 0;
		if (ct > OBMINSP)
			signal(iptr->osem);
		else if	( ++(iptr->odsend) == OBMINSP) {
			iptr->odsend = 0;
			signaln(iptr->osem, OBMINSP);
		}
	} else
	        ttyostop(iptr);

	restore(ps);
}

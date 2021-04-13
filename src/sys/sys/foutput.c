/* foutput.c - foutput */

#include <conf.h>
#include <kernel.h>
#include <frame.h>
#include <dlc.h>

/*------------------------------------------------------------------------
 *  foutput  --  select a frame from local or forward ports and send it
 *------------------------------------------------------------------------
 */
PROCESS	foutput(netid)
	int	netid;
{
	struct	frame	*fptr;
	struct	fglob	*fgptr;

	fgptr = &fdata[netid];
	control(fgptr->foutdev, DCSETREC, getpid());
	for (fgptr->fofails = fgptr->foseq = 0 ; TRUE ; ) {
		wait(fgptr->fosem);
		if (pcount(fgptr->ffport) > 0)
			fptr = (struct frame *)preceive(fgptr->ffport);
		else
			fptr = (struct frame *)preceive(fgptr->foport);
		_frsend(fptr, fgptr);
		freebuf(fptr);
	}
}

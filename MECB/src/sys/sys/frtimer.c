/* frtimer.c - frtimer */

#include <conf.h>
#include <kernel.h>
#include <frame.h>

/*------------------------------------------------------------------------
 *  frtimer  --  countdown and send timeout message to output process
 *------------------------------------------------------------------------
 */
frtimer(netid)
int	netid;
{
	struct	fglob	*fgptr;

	for (fgptr = &fdata[netid]; TRUE ; ) {
		for (fgptr->ftfuse++ ; --fgptr->ftfuse > 0 ; )
			sleep10(10);
		if (fgptr->ftfuse == 0)
			send(fgptr->ftpid, FRTMSG);
		suspend(getpid());
	}
}

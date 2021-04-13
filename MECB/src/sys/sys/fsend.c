/* fsend.c - fsend */

#include <conf.h>
#include <kernel.h>
#include <frame.h>
#ifndef	NNETS
#define	NNETS	0
#endif

/*------------------------------------------------------------------------
 *  fsend  --  enqueue a message for transmission to another machine
 *------------------------------------------------------------------------
 */
SYSCALL	fsend(netid, toaddr, fptr)
	int	netid;
	int	toaddr;
	struct frame *fptr;
{
	struct	fglob	*fgptr;

	if (netid < 0 || netid >= NNETS)
		return(SYSERR);
	fgptr = &fdata[netid];
	setfa(fptr, toaddr, fgptr->fmachid);
	psend(fgptr->foport, fptr);
	signal(fgptr->fosem);
	return(OK);
}

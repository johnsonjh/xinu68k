/* frinit.c - frinit */

#include <conf.h>
#include <kernel.h>
#include <frame.h>

#ifdef	NNETS
struct	fglob	fdata[NNETS];
int	frbpool;
#endif

/*------------------------------------------------------------------------
 *  frinit  --  initialize frame-level network input and output processes
 *------------------------------------------------------------------------
 */
SYSCALL	frinit(netid, indev, outdev)
	int	netid;
	int	indev, outdev;
{
#ifdef	NNETS
	struct	fglob	*fgptr;
	int	frtimer(), finput(), foutput();

	if (netid < 0 || netid >= NNETS)
		return(SYSERR);
	fgptr = &fdata[netid];
	fgptr->findev = indev;
	fgptr->foutdev = outdev;
	fgptr->fiport = pcreate(FIPORTS);
	fgptr->foport = pcreate(FOPORTS);
	fgptr->ffport = pcreate(FFPORTS);
	fgptr->fosem = screate(0);
	fgptr->ftfuse = -1;
	fgptr->ftpid = 0;
	fgptr->fmachid = getmid(netid);
	fgptr->ftimproc = 
		create(frtimer, FRSTACK, FRTPRIO, "frtimer", 1, netid);
	resume( create(finput, FRSTACK, FRIPRIO, "finput", 1, netid) );
	resume( create(foutput,FRSTACK, FROPRIO, "foutpt", 1, netid) );
	return(OK);
#else
	return(SYSERR);
#endif
}

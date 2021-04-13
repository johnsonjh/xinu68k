/* rfsend.c - rfsend */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  rfsend  --  send message to remote server and await reply
 *------------------------------------------------------------------------
 */
rfsend(fptr, reqlen, rplylen)
struct	fphdr	*fptr;
int	reqlen;
int	rplylen;
{
	STATWORD ps;    
	int	trys;
	int	ret;

	/* Clear server queue, and send packet to it */

	if (Rf.device == RCLOSED) {
		Rf.device = open(INTERNET, RSERVER, ANYLPORT);
		if (Rf.device == SYSERR ||
		    control(Rf.device, DG_SETMODE, DG_DMODE|DG_TMODE)
		    == SYSERR)
			return(SYSERR);
	}
	disable(ps);
	control(Rf.device, DG_CLEAR);
	for (trys=0 ; trys<RMAXTRY ; trys++) {
		if ( write(Rf.device, fptr, reqlen)
			== SYSERR) {
			restore(ps);
			return(SYSERR);
		}
		if ( (ret=read(Rf.device, fptr, rplylen) )
		    !=SYSERR && ret != TIMEOUT) {
			restore(ps);
			return(ret);
		}
	}
	restore(ps);
	return(SYSERR);
}

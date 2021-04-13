/* dgmopen.c - dgmopen */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  dgmopen  -  open a fresh datagram pseudo device and return descriptor
 *------------------------------------------------------------------------
 */
dgmopen(devptr, forport, locport)
struct	devsw	*devptr;
char	*forport;
int	locport;
{
	STATWORD ps;    
	struct	dgblk	*dgptr;
	struct	netq	*nqptr;
	int	slot;
	int	nq;
	int	i;

	disable(ps);
	if ( (slot=dgalloc()) == SYSERR) {
		restore(ps);
		return(SYSERR);
	}
	dgptr = &dgtab[slot];
	if (locport == ANYLPORT)
		locport = udpnxtp();
	else {
		for (i=0 ; i<NETQS ; i++)
			if (Net.netqs[i].valid && 
				Net.netqs[i].uport == locport) {
				dgptr->dg_state = DG_FREE;
				restore(ps);
				return(SYSERR);
			}
	}
	if (dgparse(dgptr,forport)==SYSERR || (nq=nqalloc())==SYSERR ) {
		dgptr->dg_state = DG_FREE;
		restore(ps);
		return(SYSERR);
	}
	nqptr = &Net.netqs[nq];
	nqptr->uport = dgptr->dg_lport = locport;
	dgptr->dg_xport = nqptr->xport;
	dgptr->dg_netq = nq;
	dgptr->dg_mode = DG_NMODE;
	restore(ps);
	return(dgptr->dg_dnum);
}

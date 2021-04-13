/* netinit.c - netinit */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  netinit  -  initialize network data structures
 *------------------------------------------------------------------------
 */
netinit()
{
	struct	netq	*nqptr;
	int	i;

	/* Initialize pool of network buffers and rest of Net structure	*/

	if (clkruns == FALSE)
		panic("net: no clock");
	Net.netpool = mkpool(EMAXPAK, NETBUFS);
	for (i=0 ; i<NETQS ; i++) {
		nqptr = &Net.netqs[i];
		nqptr->valid = FALSE;
		nqptr->uport = -1;
		nqptr->xport = pcreate(NETQLEN);
	}
	Net.mnvalid = Net.mavalid = FALSE;
	dot2ip(Net.gateway, GATEWAY);
	Net.nxtprt = ULPORT;
	Net.nmutex = screate(1);
	Net.npacket = Net.ndrop = Net.nover = Net.nmiss = Net.nerror = 0;
	return(OK);
}

struct	netinfo	Net;






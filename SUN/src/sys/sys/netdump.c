/* netdump.c - netdump */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  netdump  -  dump the contents of Net struct and  udp demux queues
 *------------------------------------------------------------------------
 */
netdump()
{
	int	i;
	struct	netq	*nqptr;

	kprintf("Network: buffer pool=%d, mutex=%d, next udp #=%d,",
		Net.netpool, Net.nmutex, Net.nxtprt);
	kprintf(" addr is %svalid\n", Net.mavalid?"":"in");
	kprintf("Packets: recvd=%d, tossed=%d (%d for queue overflow)\n",
		Net.npacket, Net.ndrop, Net.nover);
	for (i=0; i<NETQS; i++) {
		nqptr = &Net.netqs[i];
		if (nqptr->valid)
		  kprintf("%2d. uport=%4d, pid=%3d, xprt=%2d, size=%2d\n",
			i, nqptr->uport, nqptr->pid, nqptr->xport,
			pcount(nqptr->xport) );
	}
	return(OK);
}

/* udpnxtp.c - udpnxtp */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  udpnxtp  -  return the next available UDP local "port" number
 *------------------------------------------------------------------------
 */
udpnxtp()
{
	STATWORD ps;    
	int	i;
	int	try;
	Bool	inuse;
	struct	netq	*nqptr;

	disable(ps);
	for (inuse=TRUE ; inuse ; ) {
		inuse = FALSE;
		try = Net.nxtprt++;
		for (i=0 ; i<NETQS ; i++)
			 if ( (nqptr= &Net.netqs[i])->valid &&
				nqptr->uport == try) {
				inuse = TRUE;
				break;
			}
	}
	restore(ps);
	return(try);
}

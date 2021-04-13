/* sndrarp.c - sndrarp */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  sndrarp  -  broadcast a RARP packet to obtain my IP address
 *------------------------------------------------------------------------
 */
sndrarp()
{
	STATWORD ps;    
	struct	epacket	*mkarp();
	struct	epacket	*packet;
	int	i;
	int	mypid;
	int	resp;
	IPaddr	junk; /* needed for argument to mkarp; not ever used */

	mypid = getpid();
	for (i=0 ; i<AR_RTRY ; i++) {
		packet = mkarp(EP_RARP, AR_RREQ, junk, junk);
		if ( ((int)packet) == SYSERR)
			break;
		disable(ps);
		Arp.rarppid = mypid;
		recvclr();
		write(ETHER, packet, EMINPAK);
		resp = recvtim(AR_TIME);
		restore(ps);
		if (resp != TIMEOUT)
			return(OK);
	}
	panic("No response to RARP");
	return(SYSERR);
}

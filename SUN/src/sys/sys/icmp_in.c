/* icmp_in.c - icmp_in */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  icmp_in  -  handle ICMP packet coming in from the network
 *------------------------------------------------------------------------
 */
icmp_in(packet, icmpp, lim)
struct	epacket	*packet;
int	icmpp;
int	lim;
{
	struct	ip	*ipptr;
	struct	icmp	*icmpptr;
	int	len;

	ipptr = (struct ip *)packet->ep_data;
	icmpptr = (struct icmp *) ipptr->i_data;
	if (!Net.mavalid || icmpptr->ic_typ != ICRQECH) {
		freebuf(packet);
	} else {
		icmpptr->ic_typ = (char) ICRPECH;
		blkcopy(ipptr->i_dest, ipptr->i_src, IPLEN);
		len = net2hs(ipptr->i_paclen) - IPHLEN;
		if (isodd(len)) {
			ipptr->i_data[len++] = NULLCH;
		}
		icmpptr->ic_cksum = 0;
		icmpptr->ic_cksum = cksum(icmpptr, len>>1);
		if (pcount(icmpp) < lim) {
			psend(icmpp, packet);
		}
		else {
			freebuf(packet);
		}
	}
	return(OK);
}

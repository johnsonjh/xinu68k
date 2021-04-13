/* udpsend.c - udpsend */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  udpsend  -  send one UDP datagram to a given (foreign) IP address
 *------------------------------------------------------------------------
 */
udpsend(faddr, fport, lport, packet, datalen)
IPaddr	faddr;
short	fport;
short	lport;
struct	epacket	*packet;
int	datalen;
{
	register struct	udp	*udpptr;
	register struct	ip	*ipptr;

	/* Fill in UDP header; pass to ipsend to fill in IP header */

	ipptr = (struct ip *) packet->ep_data;
	ipptr->i_proto = IPRO_UDP;
	udpptr = (struct udp *) ipptr->i_data;
	udpptr->u_sport = hs2net(lport);
	udpptr->u_dport = hs2net(fport);
	udpptr->u_udplen = hs2net(UHLEN+datalen);
	if (isodd(datalen))
		udpptr->u_data[datalen] = (char)0;
	udpptr->u_ucksum = 0;
	return( ipsend(faddr, packet, UHLEN+datalen) );
}

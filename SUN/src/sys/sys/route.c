/* route.c - route */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  route  -  route a datagram to a given IP address
 *------------------------------------------------------------------------
 */
route(faddr, packet, totlen)
IPaddr	faddr;
struct	epacket	*packet;
int	totlen;
{
	int	result;
	int	dev;
	struct	arpent	*arpptr;
	IPaddr	mynet, destnet;

	/* If IP address is broadcast address for my network, then use	*/
	/* physical broadcast address.  Otherwise, establish a path to	*/
	/* the destination directly or through a gateway		*/

	getnet(mynet);
	netnum(destnet, faddr);
	wait(Net.nmutex);
	/* NOTE: This code uses host 0 as broadcast like 4.2bsd UNIX */
	if ( blkequ(mynet, faddr, IPLEN) ) {
		dev = ETHER;
		blkcopy(packet->ep_hdr.e_dest, EBCAST, EPADLEN);
	} else {
		if (!blkequ(destnet, mynet, IPLEN))
			faddr = Net.gateway;
		arpptr = &Arp.arptab[ getpath(faddr) ];
		if (arpptr->arp_state != AR_RSLVD) {
			arpptr->arp_state = AR_RGATE;
			arpptr = &Arp.arptab[getpath(Net.gateway)];
			if (arpptr->arp_state != AR_RSLVD) {
				panic("route - Cannot reach gateway");
				freebuf(packet);
				signal(Net.nmutex);
				return(SYSERR);
			}
		}
		dev = arpptr->arp_dev;
		blkcopy(packet->ep_hdr.e_dest, arpptr->arp_Ead, EPADLEN);
	}
	result = write(dev, packet, totlen);
	signal(Net.nmutex);
	return(result);
}

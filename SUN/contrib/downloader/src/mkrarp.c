/* mkrarp.c - mkrarp */

#include "ether.h"
#include "network.h"
#include "ip.h"
#include "udp.h"
#include "arp.h"

extern Eaddr	myeaddr;

/*------------------------------------------------------------------------
 *  mkarp  -  allocate and fill in a RARP or ARP packet
 *------------------------------------------------------------------------
 */
mkarp(buf, typ, op, myeaddr, spaddr, tpaddr)
char    *buf;
short	typ;
short	op;
IPaddr	spaddr;
IPaddr	tpaddr;
{
	register struct	arppak	*apacptr;
	struct	epacket	*packet;

	packet = (struct epacket *) buf;
	blkcopy(packet->ep_hdr.e_dest, EBCAST, AR_HLEN);
	packet->ep_hdr.e_ptype = hs2net(typ);
	apacptr = (struct arppak *) packet->ep_data;
	apacptr->ar_hrd = hs2net(AR_HRD);
	apacptr->ar_prot = hs2net(AR_PROT);
	apacptr->ar_hlen = AR_HLEN;
	apacptr->ar_plen = AR_PLEN;
	apacptr->ar_op = hs2net(op);
	blkcopy(apacptr->ar_sha, myeaddr, AR_HLEN);
	blkcopy(apacptr->ar_spa, spaddr, AR_PLEN);
	blkcopy(apacptr->ar_tha, myeaddr, AR_HLEN);
	blkcopy(apacptr->ar_tpa, tpaddr, AR_PLEN);
}

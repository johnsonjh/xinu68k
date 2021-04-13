/* mkarp.c - mkarp */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  mkarp  -  allocate and fill in an ARP or RARP packet
 *------------------------------------------------------------------------
 */
struct	epacket	*mkarp(typ, op, spaddr, tpaddr)
short	typ;
short	op;
IPaddr	spaddr;
IPaddr	tpaddr;
{
	register struct	arppak	*apacptr;
	struct	epacket	*packet;

	packet = (struct epacket *) getbuf(Net.netpool);
	if ( ((int)packet) == SYSERR)
		return((struct epacket *)SYSERR);
	blkcopy(packet->ep_hdr.e_dest, EBCAST, AR_HLEN);
	packet->ep_hdr.e_ptype = hs2net(typ);
	apacptr = (struct arppak *) packet->ep_data;
	apacptr->ar_hrd = hs2net(AR_HRD);
	apacptr->ar_prot = hs2net(AR_PROT);
	apacptr->ar_hlen = AR_HLEN;
	apacptr->ar_plen = AR_PLEN;
	apacptr->ar_op = hs2net(op);
	blkcopy(apacptr->ar_sha, eth[0].etpaddr, AR_HLEN);
	blkcopy(apacptr->ar_spa, spaddr, AR_PLEN);
	blkcopy(apacptr->ar_tha, eth[0].etpaddr, AR_HLEN);
	blkcopy(apacptr->ar_tpa, tpaddr, AR_PLEN);
	return(packet);
}

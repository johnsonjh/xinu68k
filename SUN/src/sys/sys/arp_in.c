/* arp_in.c - arp_in */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  arp_in  -  handle ARP packet coming in from Ethernet network
 *------------------------------------------------------------------------
 */
arp_in(packet, device)
struct	epacket	*packet;
int	device;
{
	STATWORD ps;    
	int	pid;
	short	arop;
	struct	arppak	*apacptr;
	struct	arpent	*atabptr;
	struct	etblk	*etptr;	
	
	etptr = (struct etblk *) devtab[device].dvioblk;
	apacptr = (struct arppak *) packet->ep_data;
	atabptr = &Arp.arptab[arpfind(apacptr->ar_spa)];
	if (atabptr->arp_state != AR_RSLVD) {
		blkcopy(atabptr->arp_Ead, apacptr->ar_sha, EPADLEN);
		atabptr->arp_dev = device;
		atabptr->arp_state = AR_RSLVD;
#ifdef DEBUG		
	        kprintf("arp_in: resolved addr for %d.%d.%d.%d\n",
			apacptr->ar_spa[0]&0xff,
			apacptr->ar_spa[1]&0xff,
			apacptr->ar_spa[2]&0xff,
			apacptr->ar_spa[3]&0xff);
#endif		
	}
	arop = net2hs(apacptr->ar_op);
	switch (arop) {

	    case AR_REQ:	/* request - answer if for me */
		if (! blkequ(Net.myaddr, apacptr->ar_tpa, IPLEN)) {
			freebuf(packet);
			return(OK);
		}
		apacptr->ar_op = hs2net(AR_RPLY);
		blkcopy(apacptr->ar_tpa, apacptr->ar_spa, IPLEN);
		blkcopy(apacptr->ar_tha, packet->ep_hdr.e_src, EPADLEN);
		blkcopy(packet->ep_hdr.e_dest, apacptr->ar_tha, EPADLEN);
		blkcopy(apacptr->ar_sha, etptr->etpaddr, EPADLEN);
		blkcopy(apacptr->ar_spa, Net.myaddr, IPLEN);
		write(device, packet, EMINPAK);
		return(OK);

	    case AR_RPLY:	/* reply - awaken requestor if any */
		disable(ps);
		pid = Arp.arppid;
		if (!isbadpid(pid)
		    && blkequ(Arp.arpwant, apacptr->ar_spa, IPLEN)) {
			Arp.arppid = BADPID;
			send(pid, OK);
		}
		freebuf(packet);
		restore(ps);
		return(OK);

	    default:
		Net.ndrop++;
		freebuf(packet);
		return(SYSERR);
	}
}

/* netin.c */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <mark.h>
#include <bufpool.h>

/*------------------------------------------------------------------------
 *  netin - initialize net, start output side, and become input daemon
 *------------------------------------------------------------------------
 */
PROCESS	netin(userpid)
int	userpid;			/* user process to resume	*/
{
	register struct	epacket	*packet;
	struct	epacket	*getbuf();
	int	icmpp;

	arpinit();
	netinit();
	icmpp = pcreate(NETFQ);
	
	resume(create(NETOUT, NETOSTK, NETIPRI-1, NETONAM,
			2, userpid,icmpp));

	for (packet = getbuf(Net.netpool) ; TRUE ;) {
		Net.npacket++;
		if ( read(ETHER,packet,sizeof(*packet)) == SYSERR ) {
#ifdef DEBUG		    
			kprintf("bad return from read\n");
#endif			
			Net.ndrop++;
			continue;
		}
		switch ((unsigned short) net2hs(packet->ep_hdr.e_ptype)) {

		    case EP_ARP:
#ifdef DEBUG		  
		        kprintf("ARP\n");
#endif			
			arp_in(packet, ETHER);
			packet = getbuf(Net.netpool);
			break;

		    case EP_RARP:
#ifdef DEBUG
			      kprintf("RARP\n");
#endif			
			rarp_in(packet, ETHER);
			packet = getbuf(Net.netpool);
			break;

		    case EP_IP:
#ifdef DEBUG
			      kprintf("IP\n");
#endif			
			ip_in(packet, icmpp, NETFQ);
			packet = getbuf(Net.netpool);
			break;

		    default: /* just drop packet */
#ifdef DEBUG
			kprintf("unknown packet type 0x%x\n",
				(unsigned short) net2hs(packet->ep_hdr.e_ptype));
#endif				
			Net.ndrop++;
		}
	}
}

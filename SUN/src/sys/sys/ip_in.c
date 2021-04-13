/* ip_in.c - ip_in */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  ip_in  -  handle IP packet coming in from the network
 *------------------------------------------------------------------------
 */
ip_in(packet, icmpp, lim)
struct	epacket	*packet;
int	icmpp;
int	lim;
{
	STATWORD ps;    
	struct	udp	*udpptr;
	struct	ip	*ipptr;
	struct	netq	*nqptr;
	int	dport;
	int	i;
	int	to;

	ipptr = (struct ip *)packet->ep_data;
	switch (ipptr->i_proto) {

	case IPRO_ICMP:		/* ICMP: pass to icmp input routine */
#ifdef DEBUG
	        kprintf("    ICMP\n");
#endif		
		return(icmp_in(packet, icmpp, lim));

	case IPRO_UDP:		/* UDP: demultiplex based on UDP "port"	*/
#ifdef DEBUG
		      kprintf("    UDP\n");
#endif		
		udpptr = (struct udp *) ipptr->i_data;
		dport = net2hs(udpptr->u_dport);
		for (i=0 ; i<NETQS ; i++) {
			nqptr = &Net.netqs[i];
			if (nqptr->uport == dport) {
				/* drop instead of blocking on psend */ 
				if (pcount(nqptr->xport) >= NETQLEN) {
					Net.ndrop++;
					Net.nover++;
					freebuf(packet);
					return(SYSERR);
				}
				psend(nqptr->xport, packet);
				disable(ps);
				to = nqptr->pid;
				if ( !isbadpid(to) ) {
				    nqptr->pid = BADPID;
				    send(to, OK);
				}
				restore(ps);
				return(OK);
			}
		}
		break;

	default:
		break;		
	}
	Net.ndrop++;
	freebuf(packet);
	return(OK);
}

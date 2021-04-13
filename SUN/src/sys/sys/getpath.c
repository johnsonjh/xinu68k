/* getpath.c - getpath */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  getpath  -  find a path (route table entry) for a given IP address
 *------------------------------------------------------------------------
 */
getpath(faddr)
IPaddr	faddr;
{
	STATWORD ps;    
	int	i;
	int	arindex;		/* route table entry index	*/
	int	mypid;			/* local copy of my process id	*/
	IPaddr	myaddr;			/* my IP address		*/
	register struct	arpent	*arpptr;
	register struct	arppak	*apacptr;
	struct	epacket	*packet, *mkarp();

	wait(Arp.arpsem);
	arpptr = &Arp.arptab[ arindex = arpfind(faddr) ];
	if (arpptr->arp_state != AR_ALLOC) {
		signal(Arp.arpsem);
		return(arindex);
	}

	/* Use ARP to obtain and record IP-to-Ether binding */
	
	getaddr(myaddr);
	mypid = getpid();
	for (i=0 ; i < AR_RTRY ; i++) {
		packet = mkarp(EP_ARP, AR_REQ, myaddr, faddr);
		blkcopy(Arp.arpwant, faddr, AR_PLEN);
		disable(ps);
		Arp.arppid = mypid;
		recvclr();
		write(ETHER, packet, EMINPAK);
		restore(ps);
		if (recvtim(AR_TIME) == OK)
			break;
	}
	Arp.arppid = BADPID;
	signal(Arp.arpsem);
	return(arindex);
}

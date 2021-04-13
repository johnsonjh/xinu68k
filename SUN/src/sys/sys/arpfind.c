/* arpfind.c - arpfind */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  arpfind  -  find or insert entry in ARP cache and return its index
 *------------------------------------------------------------------------
 */
arpfind(faddr)
IPaddr	faddr;
{
	int	i;
	int	arindex;
	struct	arpent	*atabptr;

	for (arindex=0; arindex<Arp.atabsiz; arindex++) {
		atabptr = &Arp.arptab[arindex];
		if (blkequ(atabptr->arp_Iad, faddr, IPLEN)
			&& atabptr->arp_state != AR_FREE)
			return(arindex);
	}
	if (Arp.atabsiz < AR_TAB) {
		Arp.atabsiz++;
	}
	arindex = Arp.atabnxt++;
	if (Arp.atabnxt >= AR_TAB)
		Arp.atabnxt = 0;
	atabptr = &Arp.arptab[arindex];
	atabptr->arp_state = AR_ALLOC;
	blkcopy(atabptr->arp_Iad, faddr, IPLEN);
	for(i=0 ; i<EPADLEN ; i++)
		atabptr->arp_Ead[i] = '\0';
	atabptr->arp_dev = -1;
	return(arindex);
}

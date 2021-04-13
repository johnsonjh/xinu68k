/* arpinit.c - arpinit */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  arpinit  -  initialize data structures for ARP processing
 *------------------------------------------------------------------------
 */
arpinit()
{
	struct	arpent	*atabptr;
	int	i, j;

	Arp.atabsiz = 0;
	Arp.atabnxt = 0;
	Arp.arpsem = screate(1);
	Arp.rarpsem= screate(1);
	Arp.arppid = Arp.rarppid = BADPID;
	for (i=0 ; i<AR_TAB ; i++) {
		atabptr = &Arp.arptab[i];
		atabptr->arp_state = AR_FREE;
		for (j=0 ; j<EPADLEN ; j++)
			atabptr->arp_Ead[j] = '\0';
		atabptr->arp_dev = -1;
	}
}

struct	arpblk	Arp;

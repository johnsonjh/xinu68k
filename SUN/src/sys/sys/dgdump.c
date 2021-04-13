/* dgdump.c - dgdump */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  dgdump  -  dump the datagram pseudo device control blocks
 *------------------------------------------------------------------------
 */
dgdump()
{
	struct	dgblk	*dgptr;
	int	i;

	for (i=0 ; i<Ndg ; i++) {
		dgptr = &dgtab[i];
		if (dgptr->dg_state == DG_FREE)
			continue;
		kprintf("Dev=%2d: ",dgptr->dg_dnum);
		kprintf("lport=%4d, fport=%4d, ", dgptr->dg_lport,
			dgptr->dg_fport);
		kprintf("mode=%03o, xport=%2d addr=%d.%d.%d.%d\n",
			dgptr->dg_mode, dgptr->dg_xport,
			dgptr->dg_faddr[0] &0377,
			dgptr->dg_faddr[1] &0377,
			dgptr->dg_faddr[2] &0377,
			dgptr->dg_faddr[3] &0377);
	}
	return(OK);
}

/* mkdpool.c - mkdpool */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <bufpool.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  mkdpool  --  allocate memory for a buffer pool in DMA space
 *              and link together
 *------------------------------------------------------------------------
 */
mkdpool(bufsiz, numbufs)
int	bufsiz, numbufs;
{
	STATWORD ps;    
	int	poolid;
	char	*where;
	int	*getmem();

#ifdef	MEMMARK
	if ( unmarked(bpmark) )
		poolinit();
#endif
	disable(ps);
	if (bufsiz<BPMINB || bufsiz>BPMAXB
	    || numbufs<1 || numbufs>BPMAXN
	    || nbpools >= NBPOOLS
	    || (where= (char *) getdmem((bufsiz+sizeof(int))*numbufs)) == (char *) SYSERR) {
		restore(ps);
		return(SYSERR);
	}
	poolid = nbpools++;
	bptab[poolid].bpnext = where;
	bptab[poolid].bpsize = bufsiz;
	bptab[poolid].bpsem = screate(numbufs);
	bufsiz+=sizeof(int);
	for (numbufs-- ; numbufs>0 ; numbufs--,where+=bufsiz)
		*( (int *) where ) = (int)(where+bufsiz);
	*( (int *) where) = (int) NULL;
	restore(ps);
	return(poolid);
}

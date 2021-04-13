/* mkpool.c - mkpool */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <bufpool.h>

/*------------------------------------------------------------------------
 *  mkpool  --  allocate memory for a buffer pool and link together
 *------------------------------------------------------------------------
 */
mkpool(bufsiz, numbufs)
int	bufsiz, numbufs;
{
	int	poolid;
	char	*where;

#ifdef	DEBUG
	dotrace("mkpool", &bufsiz, 2);
#endif
#ifdef	MEMMARK
	if ( unmarked(bpmark) )
		poolinit();
#endif
	disable();
	if (bufsiz<BPMINB || bufsiz>BPMAXB
	    || numbufs<1 || numbufs>BPMAXN
	    || nbpools >= NBPOOLS
	    || (where=getmem((bufsiz+sizeof(int))*numbufs)) == SYSERR) {
		restore();
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
	restore();
	return(poolid);
}

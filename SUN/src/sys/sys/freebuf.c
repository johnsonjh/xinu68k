/* freebuf.c - freebuf */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <bufpool.h>

/*------------------------------------------------------------------------
 *  freebuf  --  free a buffer that was allocated from a pool by getbuf
 *------------------------------------------------------------------------
 */
freebuf(buf)
int *buf;
{
	STATWORD ps;    
	int	poolid;

#ifdef	MEMMARK
	if ( unmarked(bpmark) )
		return(SYSERR);
#endif
	poolid = *(--buf);
	if (poolid<0 || poolid>=nbpools)
		return(SYSERR);
	disable(ps);
	*buf = (int) bptab[poolid].bpnext;
	bptab[poolid].bpnext = (char *) buf;
	restore(ps);
	signal(bptab[poolid].bpsem);
	return(OK);
}

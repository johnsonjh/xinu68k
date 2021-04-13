/* getbuf.c - getbuf */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <bufpool.h>

/*------------------------------------------------------------------------
 *  getbuf  --  get a buffer from a preestablished buffer pool
 *------------------------------------------------------------------------
 */
int *getbuf(poolid)
int poolid;
{
	STATWORD ps;    
	int	*buf;

#ifdef	MEMMARK
	if ( unmarked(bpmark) )
		return((int *) SYSERR);
#endif
	if (poolid<0 || poolid>=nbpools)
		return((int *) SYSERR);
	wait(bptab[poolid].bpsem);
	disable(ps);
	buf = (int *) bptab[poolid].bpnext;
	bptab[poolid].bpnext = (char *) *buf;
	restore(ps);
	*buf++ = poolid;
	return( (int *) buf );
}

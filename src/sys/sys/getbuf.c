/* getbuf.c - getbuf */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <bufpool.h>

/*------------------------------------------------------------------------
 *  getbuf  --  get a buffer from a preestablished buffer pool
 *------------------------------------------------------------------------
 */
char	*
getbuf(poolid)
int poolid;
{
	int	*buf;

#ifdef	DEBUG
	dotrace("getbuf", &poolid, 1);
#endif
#ifdef	MEMMARK
	if ( unmarked(bpmark) )
		return(SYSERR);
#endif
	if (poolid<0 || poolid>=nbpools)
		return(SYSERR);
	wait(bptab[poolid].bpsem);
	disable();
	buf = (int *)bptab[poolid].bpnext;
	bptab[poolid].bpnext = *buf;
	restore();
	*buf++ = poolid;
	return( (char *)buf );
}

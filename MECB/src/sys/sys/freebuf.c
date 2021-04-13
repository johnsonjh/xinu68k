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
	int	poolid;

#ifdef	DEBUG
	dotrace("freebuf", &buf, 1);
#endif
#ifdef	MEMMARK
	if ( unmarked(bpmark) )
		return(SYSERR);
#endif
	poolid = *(--buf);
	if (poolid<0 || poolid>=nbpools) {
		kprintf("Bad poolid\n");
		regdump();
		return(SYSERR);
	}
	disable();
	*buf = bptab[poolid].bpnext;
	bptab[poolid].bpnext = buf;
	restore();
	signal(bptab[poolid].bpsem);
	return(OK);
}

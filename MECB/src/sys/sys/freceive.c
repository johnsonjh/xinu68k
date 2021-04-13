/* freceive.c - freceive */

#include <conf.h>
#include <kernel.h>
#include <frame.h>

#ifndef	NNETS
#define NNETS 0
#endif

/*------------------------------------------------------------------------
 *  freceive  --  receive the next frame that arrives from a net.
 *------------------------------------------------------------------------
 */
SYSCALL	freceive(netid)
	int	netid;
{
	if (netid < 0 || netid >= NNETS)
		return(SYSERR);
	return( preceive(fdata[netid].fiport) );
}

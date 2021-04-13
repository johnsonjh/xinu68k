/* pdelete.c - pdelete */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <ports.h>

/*------------------------------------------------------------------------
 *  pdelete  --  delete a port, freeing waiting processes and messages
 *------------------------------------------------------------------------
 */
SYSCALL	pdelete(portid, dispose)
	int	portid;
	int	(*dispose)();
{
	struct	pt *ptptr;

#ifdef	DEBUG
	dotrace("pdelete", &portid, 2);
#endif
	disable();
	if ( isbadport(portid) ||
#ifdef	MEMMARK
	     unmarked(ptmark) ||
#endif
	     (ptptr= &ports[portid])->ptstate != PTALLOC ) {
		restore();
		return(SYSERR);
	}
	_ptclear(ptptr, PTFREE, dispose);
	restore();
	return(OK);
}

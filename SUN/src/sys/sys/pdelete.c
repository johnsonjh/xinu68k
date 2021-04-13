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
	STATWORD ps;    
	struct	pt *ptptr;

	disable(ps);
	if ( isbadport(portid) ||
#ifdef	MEMMARK
	     unmarked(ptmark) ||
#endif
	     (ptptr= &ports[portid])->ptstate != PTALLOC ) {
		restore(ps);
		return(SYSERR);
	}
	_ptclear(ptptr, PTFREE, dispose);
	restore(ps);
	return(OK);
}

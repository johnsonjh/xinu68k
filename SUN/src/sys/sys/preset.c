/* preset.c - preset */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <ports.h>

/*------------------------------------------------------------------------
 *  preset  --  reset a port, freeing waiting processes and messages
 *------------------------------------------------------------------------
 */
SYSCALL	preset(portid, dispose)
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
	_ptclear(ptptr, PTALLOC, dispose);
	restore(ps);
	return(OK);
}

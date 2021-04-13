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
	struct	pt *ptptr;

#ifdef	DEBUG
	dotrace("preset", &portid, 2);
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
	_ptclear(ptptr, PTALLOC, dispose);
	restore();
	return(OK);
}

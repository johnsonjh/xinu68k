/* pcount.c - pcount */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <ports.h>

/*------------------------------------------------------------------------
 *  pcount  --  return the count of current messages in a port
 *------------------------------------------------------------------------
 */
SYSCALL	pcount(portid)
int	portid;
{
	int	scnt;
	int	count;
	struct	pt	*ptptr;

#ifdef	DEBUG
	dotrace("pcount", &portid, 1);
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
	count = scount(ptptr->ptrsem);
	if ( (scnt=scount(ptptr->ptssem)) < 0 )
		count -= scnt;			/* add number waiting	*/
	restore();
	return(count);
}

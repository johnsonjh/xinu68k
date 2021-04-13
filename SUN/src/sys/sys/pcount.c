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
	STATWORD ps;    
	int	scnt;
	int	count;
	struct	pt	*ptptr;

	disable(ps);
	if ( isbadport(portid) ||
#ifdef	MEMMARK
		unmarked(ptmark) ||
#endif
		(ptptr= &ports[portid])->ptstate != PTALLOC ) {
			restore(ps);
			return(SYSERR);
	}
	count = scount(ptptr->ptrsem);
	if ( (scnt=scount(ptptr->ptssem)) < 0 )
		count -= scnt;			/* add number waiting	*/
	restore(ps);
	return(count);
}

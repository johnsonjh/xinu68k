/* getmid.c - getmid */

#include <conf.h>
#include <kernel.h>
#include <frame.h>

#define	MACHID	1		/* temporary value for testing		*/
#ifndef	NNETS
#define	NNETS	0
#endif

/*------------------------------------------------------------------------
 *  getmid  --  get the machine id for this machine on a given net
 *------------------------------------------------------------------------
 */
SYSCALL	getmid(netid)
	int	netid;
{
	int	id;
	char	ch;

	if (netid < 0 || netid > NNETS)
		return(SYSERR);

	return(MACHID); /* replace by value from ROM if feasible */
}

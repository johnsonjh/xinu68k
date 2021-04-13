/* sendf.c - sendf */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  sendf  --  sendf a message to another process, forcing delivery
 *------------------------------------------------------------------------
 */
SYSCALL	sendf(pid, msg)
int	pid;
WORD	msg;
{
	struct	pentry	*pptr;

	disable();
	if (isbadpid(pid)||((pptr= &proctab[pid])->pstate == PRFREE)) {
		restore();
		return(SYSERR);
	}
	pptr->pmsg = msg;
	pptr->phasmsg++;
	if (pptr->pstate == PRRECV)
		ready(pid, RESCHYES);
	return(OK);
}

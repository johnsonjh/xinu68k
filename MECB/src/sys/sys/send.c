/* send.c - send */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  send  --  send a message to another process
 *------------------------------------------------------------------------
 */
SYSCALL	send(pid, msg)
int	pid;
WORD	msg;
{
	struct	pentry	*pptr;

	disable();
	if (isbadpid(pid) || ( (pptr= &proctab[pid])->pstate == PRFREE)
	   || pptr->phasmsg != 0) {
		restore();
		return(SYSERR);
	}
	pptr->pmsg = msg;
	pptr->phasmsg++;
	if (pptr->pstate == PRRECV)
		ready(pid, RESCHYES);
	return(OK);
}

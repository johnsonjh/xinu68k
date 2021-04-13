/* kill.c - kill */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>

/*------------------------------------------------------------------------
 * kill  --  kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
SYSCALL kill(pid)
	int	pid;			/* process to kill		*/
{
	struct	pentry	*pptr;		/* points to proc. table for pid*/

	disable();
	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate==PRFREE) {
		restore();
		return(SYSERR);
	}
	if (--numproc == 0)
		xdone();
	freestk(pptr->pbase, pptr->pstklen);
	switch (pptr->pstate) {

	case PRCURR:	pptr->pstate = PRFREE; /* suicide */
			resched();

	case PRWAIT:	semaph[pptr->psem].semcnt++;

	case PRSLEEP:
	case PRREADY:	dequeue(pid);

	default:	pptr->pstate = PRFREE;
	}
	restore();
	return(OK);
}

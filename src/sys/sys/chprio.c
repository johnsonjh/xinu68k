/* chprio.c - chprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 * chprio  --  change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
SYSCALL chprio(pid,newprio)
	int	pid;
	int	newprio;		/* newprio > 0			*/
{
	int	oldprio;
	struct	pentry	*pptr;

	disable();
	if (isbadpid(pid) || newprio<=0 ||
	    (pptr = &proctab[pid])->pstate == PRFREE) {
		restore();
		return(SYSERR);
	}
	oldprio = pptr->pprio;
	pptr->pprio = newprio;
	if (pptr->pstate == PRREADY) {
		dequeue(pid);
		insert(pid, rdyhead, newprio);
	} 
	restore();
	return(oldprio);
}

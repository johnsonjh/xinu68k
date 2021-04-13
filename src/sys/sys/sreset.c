/* sreset.c - sreset */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>

/*------------------------------------------------------------------------
 *  sreset  --  reset the count and queue of a semaphore
 *------------------------------------------------------------------------
 */
SYSCALL sreset(sem,count)
	int	sem;
	int	count;
{
	struct	sentry	*sptr;
	int	pid;
	int	slist;

#ifdef	DEBUG
	dotrace("sreset", &sem, 1);
#endif
	disable();
	if (isbadsem(sem) || count<0 || semaph[sem].sstate==SFREE) {
		restore();
		return(SYSERR);
	}
	sptr = &semaph[sem];
	slist = sptr->sqhead;
	while ((pid=getfirst(slist)) != EMPTY)
		ready(pid,RESCHNO);
	sptr->semcnt = count;
	resched();
	restore();
	return(OK);
}

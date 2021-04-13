/* sdelete.c - sdelete */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>

/*------------------------------------------------------------------------
 * sdelete  --  delete a semaphore by releasing its table entry
 *------------------------------------------------------------------------
 */
SYSCALL sdelete(sem)
	int	sem;
{
	int	pid;
	struct	sentry	*sptr;

	disable();
	if (isbadsem(sem) || semaph[sem].sstate==SFREE) {
		restore();
		return(SYSERR);
	}
	sptr = &semaph[sem];
	sptr->sstate = SFREE;
	if (nonempty(sptr->sqhead)) {
		while( (pid=getfirst(sptr->sqhead)) != EMPTY)
			ready(pid,RESCHNO);
		resched();
	}
	restore();
	return(OK);
}

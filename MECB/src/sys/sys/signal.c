/* signal.c - signal */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>

/*------------------------------------------------------------------------
 * signal  --  signal a semaphore, releasing one waiting process
 *------------------------------------------------------------------------
 */
SYSCALL signal(sem)
	int	sem;
{
	register struct	sentry	*sptr;

	disable();
	if (isbadsem(sem) || (sptr= &semaph[sem])->sstate==SFREE) {
		restore();
		return(SYSERR);
	}
	if ((sptr->semcnt++) < 0)
		ready(getfirst(sptr->sqhead), RESCHYES);
	restore();
	return(OK);
}

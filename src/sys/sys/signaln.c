/* signaln.c - signaln */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>

/*------------------------------------------------------------------------
 *  signaln -- signal a semaphore n times
 *------------------------------------------------------------------------
 */
SYSCALL signaln(sem,count)
	int	sem;
	int	count;
{
	struct	sentry	*sptr;

#ifdef	DEBUG
	dotrace("sign", &sem, 2);
#endif
	disable();
	if (isbadsem(sem) || semaph[sem].sstate==SFREE || count<=0) {
		restore();
		return(SYSERR);
	}
	sptr = &semaph[sem];
	for (; count > 0  ; count--)
		if ((sptr->semcnt++) < 0)
			ready(getfirst(sptr->sqhead), RESCHNO);
	resched();
	restore();
	return(OK);
}

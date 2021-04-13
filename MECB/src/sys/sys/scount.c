/* scount.c - scount */

#include <conf.h>
#include <kernel.h>
#include <sem.h>

extern	struct	sentry	semaph[];

/*------------------------------------------------------------------------
 *  scount  --  return a semaphore count
 *------------------------------------------------------------------------
 */
SYSCALL scount(sem)
	int	sem;
{
#ifdef	DEBUG
	dotrace("scount", &sem, 1);
#endif
	if (isbadsem(sem) || semaph[sem].sstate==SFREE)
		return(SYSERR);
	return(semaph[sem].semcnt);
}

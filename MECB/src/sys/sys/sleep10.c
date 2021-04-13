/* sleep10.c - sleep10 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>

/*------------------------------------------------------------------------
 * sleep10  --  delay the caller for a time specified in tenths of seconds
 *------------------------------------------------------------------------
 */
SYSCALL	sleep10(n)
	int	n;
{
#ifdef	DEBUG1
	dotrace("sleep10", &n, 1);
#endif
	if (n < 0  || clkruns==0)
		return(SYSERR);
	if (n == 0) {
	        disable();
		resched();
		restore();
		return(OK);
	}
        disable();
        insertd(currpid,clockq,n);
	slnempty = TRUE;
	sltop = &q[q[clockq].qnext].qkey;
        proctab[currpid].pstate = PRSLEEP;
	resched();
        restore();
	return(OK);
}

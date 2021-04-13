/* sleep.c - sleep */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>

/*------------------------------------------------------------------------
 * sleep  --  delay the calling process n seconds
 *------------------------------------------------------------------------
 */
SYSCALL	sleep(n)
	int	n;
{
#ifdef	DEBUG
	dotrace("sleep", &n, 1);
#endif
	if (n<0 || clkruns==0)
		return(SYSERR);
	if (n == 0) {
	        disable();
		resched();	/* resched assumes caller is disabled */
		restore();
		return(OK);
	}
	while (n >= 1000) {
		sleep10(10000);
		n -= 1000;
	}
	if (n > 0)
		sleep10(10*n);
	return(OK);
}

/* ssclock.c - stopclk, strtclk */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>

#ifdef	RTCLOCK

/*------------------------------------------------------------------------
 *  stopclk  --  put the clock in defer mode
 *------------------------------------------------------------------------
 */
stopclk()
{
	defclk++;
}

/*------------------------------------------------------------------------
 *  strtclk  --  take the clock out of defer mode
 *------------------------------------------------------------------------
 */
strtclk()
{
	STATWORD ps;    
	int makeup;
	int next;

	disable(ps);
	if ( defclk<=0 || --defclk>0 ) {
		restore(ps);
		return;
	}
	makeup = clkdiff;
	preempt -= makeup;
	clkdiff = 0;
	if ( slnempty ) {
		for (next=firstid(clockq) ; 
		    next < NPROC && q[next].qkey < makeup ;
		    next=q[next].qnext) {
			makeup -= q[next].qkey;
			q[next].qkey = 0;
		}
		if (next < NPROC)
			q[next].qkey -= makeup;
		wakeup();
	}
	if ( preempt <= 0 )
	        resched();
	restore(ps);
}
#endif

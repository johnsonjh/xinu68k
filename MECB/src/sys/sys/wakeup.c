/* wakeup.c - wakeup */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>

/*------------------------------------------------------------------------
 * wakeup  --  called by clock interrupt dispatcher to awaken processes
 *------------------------------------------------------------------------
 */
INTPROC	wakeup()
{
        while (nonempty(clockq) && firstkey(clockq) <= 0)
                ready(getfirst(clockq),RESCHNO);
	if ( slnempty = nonempty(clockq) )
		sltop = & q[q[clockq].qnext].qkey;
	resched();
}

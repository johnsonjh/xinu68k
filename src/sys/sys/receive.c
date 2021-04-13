/* receive.c - receive */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  receive  -  wait for a message and return it
 *------------------------------------------------------------------------
 */
SYSCALL	receive()
{
	struct	pentry	*pptr;
	WORD	msg;

	disable();
	pptr = &proctab[currpid];
	if (pptr->phasmsg == 0) {
		pptr->pstate = PRRECV;
		resched();
	}
	msg = pptr->pmsg;
	pptr->phasmsg = 0;
	restore();
	return(msg);
}

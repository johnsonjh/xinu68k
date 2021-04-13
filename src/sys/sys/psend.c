/* psend.c - psend */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <ports.h>

/*------------------------------------------------------------------------
 *  psend  --  send a message to a port by enqueuing it
 *------------------------------------------------------------------------
 */
SYSCALL	psend(portid, msg)
int	portid;
WORD	msg;
{
	struct	pt	*ptptr;
	int	seq;
	struct	ptnode	*freenode;
	

#ifdef	DEBUG
	dotrace("psend", &portid, 2);
#endif
	disable();
	if ( isbadport(portid) ||
#ifdef	MEMMARK
	     unmarked(ptmark) ||
#endif
	     (ptptr= &ports[portid])->ptstate != PTALLOC ) {
		restore();
		return(SYSERR);
	}

	/* wait for space and verify port is still allocated */

	seq = ptptr->ptseq;
	if (wait(ptptr->ptssem) == SYSERR
	    || ptptr->ptstate != PTALLOC
	    || ptptr->ptseq != seq) {
		restore();
		return(SYSERR);
	}
	if (ptfree == NULL) {
		kprintf("Ports  -  out of nodes");
		return(SYSERR);
	}
	freenode = ptfree;
	ptfree  = freenode->ptnext;
	freenode->ptnext = (struct ptnode *) NULL;
	freenode->ptmsg  = msg;
	if (ptptr->pttail == (struct ptnode *) NULL)	/* empty queue */
		ptptr->pttail = ptptr->pthead = freenode;
	else {
		(ptptr->pttail)->ptnext = freenode;
		ptptr->pttail = freenode;
	}
	signal(ptptr->ptrsem);
	restore();
	return(OK);
}

/* ptclear.c - _ptclear */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <ports.h>

/*------------------------------------------------------------------------
 *  _ptclear  --  used by pdelete and preset to clear a port
 *------------------------------------------------------------------------
 */
_ptclear(ptptr, newstate, dispose)
	struct	pt	*ptptr;
	int	newstate;
	int	(*dispose)();
{
	struct	ptnode	*p;


	/* put port in limbo until done freeing processes */

#ifdef	DEBUG
	dotrace("_ptclear", &ptptr, 3);
#endif
	ptptr->ptstate = PTLIMBO;
	ptptr->ptseq++;
	if ( (p=ptptr->pthead) != (struct ptnode *)NULL ) {
		for(; p != (struct ptnode *) NULL ; p=p->ptnext)
			(*dispose)( p->ptmsg );
		(ptptr->pttail)->ptnext = ptfree;
		ptfree = ptptr->pthead;
	}
	if (newstate == PTALLOC) {
		ptptr->pttail = ptptr->pthead = (struct ptnode *) NULL;
		sreset(ptptr->ptssem, ptptr->ptmaxcnt);
		sreset(ptptr->ptrsem, 0);
	} else {
		sdelete(ptptr->ptssem);
		sdelete(ptptr->ptrsem);
	}
	ptptr->ptstate = newstate;
}

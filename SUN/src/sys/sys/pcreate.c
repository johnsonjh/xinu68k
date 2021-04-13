/* pcreate.c - pcreate */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <ports.h>

/*------------------------------------------------------------------------
 *  pcreate  --  create a port that allows "count" outstanding messages
 *------------------------------------------------------------------------
 */
SYSCALL	pcreate(count)
int	count;
{
	STATWORD ps;    
	int	i, p;
	struct	pt	*ptptr;

	if (count < 0)
		return(SYSERR);
	disable(ps);
#ifdef	MEMMARK
	if (mark(ptmark) == OK)
		pinit(MAXMSGS);
#endif
	for (i=0 ; i<NPORTS ; i++) {
		if ( (p=ptnextp--) <= 0)
			ptnextp = NPORTS - 1;
		if ( (ptptr= &ports[p])->ptstate == PTFREE) {
			ptptr->ptstate = PTALLOC;
			ptptr->ptssem = screate(count);
			ptptr->ptrsem = screate(0);
			ptptr->pthead = ptptr->pttail = (struct ptnode *)NULL;
			ptptr->ptseq++;
			ptptr->ptmaxcnt = count;
			restore(ps);
			return(p);
		}
	}
	restore(ps);
	return(SYSERR);
}

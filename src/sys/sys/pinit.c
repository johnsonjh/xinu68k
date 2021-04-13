/* pinit.c - pinit */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <ports.h>

#ifdef	MEMMARK
MARKER	ptmark;
#endif
struct	ptnode	*ptfree;		/* list of free queue nodes	*/
struct	pt	ports[NPORTS];
int	ptnextp;

/*------------------------------------------------------------------------
 *  pinit  --  initialize all ports
 *------------------------------------------------------------------------
 */
SYSCALL	pinit(maxmsgs)
int maxmsgs;
{
	int	i;
	struct	ptnode	*next, *prev;

#ifdef	DEBUG
	dotrace("pinit", &maxmsgs, 1);
#endif
	if ( (ptfree=getmem(maxmsgs*sizeof(struct ptnode)))==SYSERR ) {
		kprintf("pinit - insufficient memory");
		return SYSERR;
	}
	for (i=0 ; i<NPORTS ; i++)
		ports[i].ptstate = PTFREE;
	ptnextp = NPORTS - 1;

	/* link up free list of message pointer nodes */

	for ( prev=next=ptfree ;  --maxmsgs > 0  ; prev=next )
		prev->ptnext = ++next;
	prev->ptnext = (struct ptnode *)NULL;
	return(OK);
}

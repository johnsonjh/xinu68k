/* getmem.c - getmem */

#include <conf.h>
#include <kernel.h>
#include <mem.h>

/*------------------------------------------------------------------------
 * getmem  --  allocate heap storage, returning lowest WORD address
 *------------------------------------------------------------------------
 */
WORD	*getmem(nbytes)
	unsigned nbytes;
{
	struct	mblock	*p, *q, *leftover;

	disable();
	if (nbytes==0 || memlist.mnext== (struct mblock *) NULL) {
		restore();
		return( (WORD *)SYSERR);
	}
	nbytes = (unsigned int) roundew(nbytes);
	for (q= &memlist,p=memlist.mnext ;
	     p != (struct mblock *) NULL ;
	     q=p,p=p->mnext)
		if ( p->mlen == nbytes) {
			q->mnext = p->mnext;
			restore();
			return( (WORD *)p );
		} else if ( p->mlen > nbytes ) {
			leftover = (struct mblock *)( (unsigned)p + nbytes );
			q->mnext = leftover;
/* eoneil 2/29/88--exchanged next two statements so getmem(4) works */
			leftover->mlen = p->mlen - nbytes;
			leftover->mnext = p->mnext;
			restore();
			return( (WORD *)p );
		}
	restore();
	return( (WORD *)SYSERR );
}

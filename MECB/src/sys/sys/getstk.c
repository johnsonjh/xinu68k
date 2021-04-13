/* getstk.c - getstk */

#include <conf.h>
#include <kernel.h>
#include <mem.h>

/*------------------------------------------------------------------------
 * getstk  --  allocate stack memory, returning address of topmost WORD
 *------------------------------------------------------------------------
 */
WORD	*getstk(nbytes)
	unsigned  int nbytes;
{
	struct	mblock	*p, *q;	/* q follows p along memlist		*/
	struct	mblock	*fits, *fitsq;
	WORD	address;

	disable();
	if (nbytes == 0) {
		restore();
		return( (WORD *)SYSERR );
	}
	nbytes = (unsigned int) roundew(nbytes);
	fits = (struct mblock *) NULL;
	q = &memlist;
	for (p = q->mnext ; p !=(struct mblock *) NULL ; q = p,p = p->mnext)
		if ( p->mlen >= nbytes) {
			fitsq = q;
			fits = p;
		}
	if (fits == (struct mblock *) NULL) {
		restore();
		return( (WORD *)SYSERR );
	}
	/* eoneil 2/29/88--moved address calc up above if */
	/* so pointing to last word in area returned */
	address = (WORD) fits + fits->mlen - sizeof(WORD);
	if (nbytes == fits->mlen) {
		fitsq->mnext = fits->mnext;
	} else {
		fits->mlen -= nbytes;
	}
/*	address = (WORD) fits;
	address += fits->mlen - sizeof(WORD); */
	restore();
	return( (WORD *) address);
}

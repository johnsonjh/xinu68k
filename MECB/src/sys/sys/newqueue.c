/* newqueue.c  -  newqueue */

#include <conf.h>
#include <kernel.h>
#include <q.h>

/*------------------------------------------------------------------------
 * newqueue  --  initialize a new list in the q structure
 *------------------------------------------------------------------------
 */
int	newqueue(hindex,tindex)
	int	*hindex,*tindex;	/* addresses of integers to	*/
					/* contain head, tail indexes	*/
{
	struct	qent	*hptr;
	struct	qent	*tptr;

	hptr = &q[(*hindex)=nextqueue++]; /* assign and rememeber queue	*/
	tptr = &q[(*tindex)=nextqueue++]; /* index values for head&tail	*/
	hptr->qnext = *tindex;
	hptr->qprev = EMPTY;
	tptr->qnext = EMPTY;
	hptr->qkey  = MININT;
	tptr->qkey  = MAXINT;
	tptr->qprev = *hindex;
}

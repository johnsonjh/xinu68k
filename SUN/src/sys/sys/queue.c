/* queue.c - dequeue, enqueue */

#include <conf.h>
#include <kernel.h>
#include <q.h>

/*------------------------------------------------------------------------
 * enqueue  --	insert an item at the tail of a list
 *------------------------------------------------------------------------
 */
int	enqueue(item, tail)
	int	item;			/* item to enqueue on a list	*/
	int	tail;			/* index in q of list tail	*/
{
	struct	qent	*tptr;		/* points to tail entry		*/
	struct	qent	*mptr;		/* points to item entry		*/

	tptr = &q[tail];
	mptr = &q[item];
	mptr->qnext = tail;
	mptr->qprev = tptr->qprev;
	q[tptr->qprev].qnext = item;
	tptr->qprev = item;
	return(item);
}


/*------------------------------------------------------------------------
 *  dequeue  --  remove an item from the head of a list and return it
 *------------------------------------------------------------------------
 */
int	dequeue(item)
	int	item;
{
	struct	qent	*mptr;		/* pointer to q entry for item	*/

	mptr = &q[item];
	q[mptr->qprev].qnext = mptr->qnext;
	q[mptr->qnext].qprev = mptr->qprev;
	return(item);
}

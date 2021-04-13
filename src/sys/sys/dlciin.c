/* dlciin.c - dlciin */

#include <conf.h>
#include <kernel.h>
#include <slu11.h>
#include <proc.h>
#include <sleep.h>
#include <dlc.h>

/*------------------------------------------------------------------------
 *  dlciin  --  dlc input interrupt handler (block + normal modes)
 *------------------------------------------------------------------------
 */
INTPROC	dlciin(dptr)
register struct	dlblk	*dptr;
{
	struct	csr *cptr;
	register char	ch;
	int	icode;

	ch = ( (icode = ((cptr=dptr->dioaddr)->crbuf)) ) & SLUCHMASK;

	switch (dptr->distate) {

	case DLIINIT:				/* non-blockmode read */
			if (ch == DLRESTART && dptr->dostate!=DLOINIT) {
				dptr->dostate = DLORESTART;
				return;
			}
			if (dptr->dpid)
				sendf(dptr->dpid, (ch&0377) );
			return;

	case DLIREADY:				/* ready for block */
			if (ch == DLSTART) {
				dptr->distate = DLIREAD;
				defclk++;
				if (currpid == NULLPROC)
					proctab[NULLPROC].pprio = MAXINT;
				else {
					insert(dequeue(NULLPROC),rdyhead,
					proctab[NULLPROC].pprio=MAXINT);
					resched();
				}
			} else {		/* unexpected character	*/
				dptr->dochar = DLRESTART;
				dptr->dovalid = TRUE;
				cptr->ctstat = SLUENABLE;
				dptr->distate = DLIWAIT;
			}
			return;

	case DLIREAD:				/* doing blockmode read	*/
			if (icode & SLUERMASK) {	/* input error	*/
				dptr->distate = DLIWAIT;
				dptr->dicount = 0;
				dptr->dinext = dptr->distart;
				dptr->diesc = FALSE;
				dptr->dochar = DLRESTART;
				dptr->dovalid = TRUE;
				cptr->ctstat = SLUENABLE;
				proctab[NULLPROC].pprio = 0;
				strtclk();
				return;
			}
			if (ch == DLEOB) {	/* end of block	*/
				dptr->distate = DLIDONE;
				ready(dptr->diproc, RESCHNO);
				proctab[NULLPROC].pprio = 0;
				strtclk();
				resched();
				return;
			}
			if (ch == DLSTART) {		/* restart read	*/
				dptr->diesc = FALSE;
				dptr->dinext = dptr->distart;
				dptr->dicount = 0;
				return;
			}
			if (dptr->diesc) {		/* map escapes	*/
				dptr->diesc = FALSE;
				if ( ch & DLESCBIT) {	/* impossible	*/
					dptr->distate = DLIWAIT;
					dptr->dicount = 0;
					dptr->dinext = dptr->distart;
					dptr->dochar = DLRESTART;
					dptr->dovalid = TRUE;
					cptr->ctstat = SLUENABLE;
					proctab[NULLPROC].pprio = 0;
					strtclk();
					return;
				} else
					ch |= DLESCBIT;
			} else if (ch == DLESC) {	/* escape next	*/
				dptr->diesc = TRUE;
				return;
			}
			*dptr->dinext++ = ch;
			if ( ++(dptr->dicount) >= dptr->dimax ) {
				dptr->distate = DLIDONE;
				ready(dptr->diproc, RESCHNO);
				proctab[NULLPROC].pprio = 0;
				strtclk();
				resched();
			}
			return;

	case DLIWAIT:
			if (ch == DLSTART) {
				dptr->distate = DLIREAD;
				defclk++;
				if (currpid == NULLPROC)
					proctab[NULLPROC].pprio = MAXINT;
				else {
					insert(dequeue(NULLPROC),rdyhead,
					proctab[NULLPROC].pprio=MAXINT);
					resched();
				}
			} else if (ch == DLEOB)
				dptr->distate = DLIREADY;
			return;

	case DLIDONE:
			return;

	default:
			panic("impossible dlc input state");
	}
}

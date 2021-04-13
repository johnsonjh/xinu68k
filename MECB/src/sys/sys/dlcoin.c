/* dlcoin.c - dlcoin */

#include <conf.h>
#include <kernel.h>
#include <slu11.h>
#include <proc.h>
#include <sleep.h>
#include <dlc.h>

/*------------------------------------------------------------------------
 *  dlcoin  --  dlc output interrupt handler
 *------------------------------------------------------------------------
 */
INTPROC	dlcoin(dptr)
register struct	dlblk	*dptr;
{
	struct	csr	*cptr;
	char	ch;

	cptr = dptr->dioaddr;

	switch (dptr->dostate) {

	case DLOINIT:				/* non-blockmode output	*/
			if (dptr->dovalid) {
				cptr->ctbuf = dptr->dochar;
				dptr->dovalid = FALSE;
			} else
				cptr->ctstat = SLUDISABLE;
			return;

	case DLOREADY:				/* ready to write block	*/
			cptr->ctbuf = DLSTART;
			dptr->dostate = DLOSTALL;
			dptr->dostall = DLNSTALL;
			dptr->doesc = FALSE;
			return;

	case DLOSTALL:				/* stalling a while	*/
			cptr->ctstat = SLUDISABLE;
			cptr->ctstat = SLUENABLE;
			if (dptr->dostall-- > 0)
				return;
			dptr->dostate = DLOWRITE;
			return;

	case DLOWRITE:				/* writing a block	*/
			if (dptr->docount-- <= 0) {
				dptr->dostate = DLODONE;
				cptr->ctbuf = DLEOB;
				return;
			}
			if (((ch= *dptr->donext++)&DLESCMASK)==DLESCPED){
				if (dptr->doesc) {
					dptr->doesc = FALSE;
					cptr->ctbuf = ch & DLEBMASK;
				} else {
					dptr->donext--;
					dptr->docount++;
					dptr->doesc = TRUE;
					cptr->ctbuf = DLESC;
				}
				return;
			}
			cptr->ctbuf = ch;
			return;

	case DLODONE:				/* finished writing	*/
			if (dptr->docount < 0) {
				ready(dptr->doproc, RESCHYES);
				dptr->docount = 0;
			}
			cptr->ctstat = SLUDISABLE;
			return;

	case DLORESTART:			/* restart transmission	*/
			dptr->dostate = DLOREADY;
			dptr->donext = dptr->dostart;
			dptr->docount = dptr->dotot;
			cptr->ctstat = SLUDISABLE;
			cptr->ctstat = SLUENABLE;
			return;

	default:
			panic("impossible dlc output state");
	}
}

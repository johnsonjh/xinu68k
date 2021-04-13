/* dlcputc.c - dlcputc */

#include <conf.h>
#include <kernel.h>
#include <slu11.h>
#include <proc.h>
#include <sleep.h>
#include <dlc.h>

/*------------------------------------------------------------------------
 *  dlcputc  --  write non-blockmode character (used for acks & resets)
 *------------------------------------------------------------------------
 */
dlcputc(devptr, ch)
struct	devsw	*devptr;
char	ch;
{
	char	ps;
	struct	dlblk	*dptr;
	struct	csr	*cptr;

	disable(ps);
	if ( (dptr = devptr->dvioblk)->dostate != DLOINIT) {
		restore(ps);
		return(SYSERR);
	}
	dptr->dochar = ch;
	dptr->dovalid = TRUE;
	(dptr->dioaddr)->ctstat = SLUENABLE;
	restore(ps);
	return(OK);
}

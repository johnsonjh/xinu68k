/* dlcwrite.c - dlcwrite */

#include <conf.h>
#include <kernel.h>
#include <slu11.h>
#include <proc.h>
#include <sleep.h>
#include <dlc.h>

/*------------------------------------------------------------------------
 *  dlcwrite  --  write a block (frame) with byte-stuffing and EOB
 *------------------------------------------------------------------------
 */
dlcwrite(devptr, buff, count)
struct	devsw	*devptr;
char	*buff;
int	count;
{
	char	ps;
	struct	dlblk	*dptr;
	struct	csr	*cptr;

	if (count < 0)
		return(SYSERR);
	else if (count == 0)
		return(OK);
	disable(ps);
	dptr = devptr->dvioblk;
	if (dptr->dostate != DLOINIT) {
		restore(ps);
		return(SYSERR);
	}
	dptr->dostate = DLOREADY;
	dptr->dostart = dptr->donext = buff;
	dptr->dotot = dptr->docount = count;
	dptr->doesc = FALSE;
	(dptr->dioaddr)->ctstat = SLUENABLE;
	dptr->doproc = currpid;
	suspend(currpid);
	dptr->dostate = DLOINIT;
	restore(ps);
	return(OK);
}

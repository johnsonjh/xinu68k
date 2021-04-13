/* dlcread.c - dlcread */

#include <conf.h>
#include <kernel.h>
#include <slu11.h>
#include <proc.h>
#include <sleep.h>
#include <dlc.h>

/*------------------------------------------------------------------------
 *  dlcread  --  read a block (frame) to user's buffer, unstuffing chars
 *------------------------------------------------------------------------
 */
dlcread(devptr, buf, maxchars)
struct	devsw	*devptr;
char	*buf;
int	maxchars;
{
	char	ps;
	int	nread;
	struct	dlblk	*dptr;

	disable(ps);
	if (maxchars<=0 || (dptr=devptr->dvioblk)->distate != DLIINIT) {
		restore(ps);
		return(SYSERR);
	}
	dptr->distate = DLIREADY;
	dptr->dinext = dptr->distart = buf;
	dptr->dimax = maxchars;
	dptr->dicount = 0;
	dptr->diesc = FALSE;
	dptr->diproc = currpid;
	suspend(currpid);
	nread = dptr->dicount;
	dptr->distate = DLIINIT;
	restore(ps);
	return(nread);
}

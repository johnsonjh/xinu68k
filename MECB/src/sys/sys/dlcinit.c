/* dlcinit.c - dlcinit */

#include <conf.h>
#include <kernel.h>
#include <slu11.h>
#include <proc.h>
#include <sleep.h>
#include <dlc.h>

struct	dlblk	dlc[Ndlc];

/*------------------------------------------------------------------------
 *  dlcinit  --  initialize dlc control block and device
 *------------------------------------------------------------------------
 */
dlcinit(devptr)
struct	devsw	*devptr;
{
	struct	dlblk	*dptr;
	struct	csr	*cptr;
	char	ps;
	int	junk;

	return;			/* to make MECB version work******** */

	disable(ps);
	dptr = &dlc[devptr->dvminor];
	devptr->dvioblk = dptr;
	iosetvec(devptr->dvnum, dptr, dptr);
	dptr->dioaddr = devptr->dvcsr;
	dptr->dostall = 0;
	dptr->distate = DLIINIT;
	dptr->dostate = DLOINIT;
	dptr->donext = dptr->dinext = NULL;
	dptr->doesc = dptr->diesc = 0;
	dptr->dpid = 0;
	dptr->dovalid = FALSE;
	dptr->diproc = dptr->doproc = -1;
	dptr->dotot = dptr->docount = 0;
	dptr->dimax = dptr->dicount = 0;
	cptr = dptr->dioaddr;		/* get device CSR address	*/
	junk = cptr->crbuf;		/* clear device receiver and	*/
	cptr->crstat = SLUENABLE;	/*   enable read interrupts	*/
	cptr->ctstat = SLUDISABLE;	/* disable write interrupts	*/
}

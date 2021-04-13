/* dlccntl.c - dlccntl */

#include <conf.h>
#include <kernel.h>
#include <slu11.h>
#include <proc.h>
#include <sleep.h>
#include <dlc.h>

/*------------------------------------------------------------------------
 *  dlccntl  --  perform control operations on dlc devices and driver
 *------------------------------------------------------------------------
 */
dlccntl(devptr, opcode, parm1)
struct	devsw	*devptr;
int	opcode;
int	parm1;
{
	struct	dlblk	*dptr;

	dptr = devptr->dvioblk;
	if (opcode == DCSETREC) {
		dptr->dpid = parm1;
		return(OK);
	} else if (opcode == DCCLRREC) {
		dptr->dpid = 0;
		return(OK);
	} else
		return(SYSERR);
}

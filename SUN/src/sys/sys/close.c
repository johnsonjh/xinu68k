/* close.c - close */

#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  close  -  close a device
 *------------------------------------------------------------------------
 */
SYSCALL	close(descrp)
int descrp;
{
	struct	devsw	*devptr;

	if (isbaddev(descrp) )
		return(SYSERR);
	devptr = &devtab[descrp];
	return( (*devptr->dvclose)(devptr));
}

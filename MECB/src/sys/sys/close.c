/* close.c - close */

#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  close  -  close a device
 *------------------------------------------------------------------------
 */
close(descrp)
int descrp;
{
	struct	devsw	*devptr;

#ifdef	DEBUG
	dotrace("close",&descrp,1);
#endif
	if (isbaddev(descrp) )
		return(SYSERR);
	devptr = &devtab[descrp];
	return( (*devptr->dvclose)(devptr));
}

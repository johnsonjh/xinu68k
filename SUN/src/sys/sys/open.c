/* open.c - open */

#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  open  -  open a connection to a device/file (parms 2 &3 are optional)
 *------------------------------------------------------------------------
 */
open(descrp, nam, mode)
int	descrp;
char	*nam;
char	*mode;
{
	struct	devsw	*devptr;

	if ( isbaddev(descrp) )
		return(SYSERR);
	devptr = &devtab[descrp];
	return(	(*devptr->dvopen)(devptr, nam, mode) );
}

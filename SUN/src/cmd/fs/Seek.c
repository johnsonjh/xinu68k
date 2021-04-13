/* Seek.c - Seek */

#include "fs.h"

/*------------------------------------------------------------------------
 *  Seek - seek in a file given its index in the file cache table
 *------------------------------------------------------------------------
 */
Seek(rfd, newpos)
int	rfd;
long	newpos;
{
	long	lseek();
	long	pos;

	if ( (pos=lseek(fcache[rfd].fd, newpos, 0)) < 0)
		return(SYSERR);
	fcache[rfd].currpos = pos;
	return(OK);
}

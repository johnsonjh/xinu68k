/* Close.c - Close */

#include "fs.h"

/*------------------------------------------------------------------------
 *  Close - close a file given its index in the file cache table
 *------------------------------------------------------------------------
 */
Close(rfd)
int	rfd;
{
	if (close(fcache[rfd].fd) < 0)
		return(SYSERR);
	fcache[rfd].name[0] = NULLCH;
	fcache[fcache[rfd].prev].next = fcache[rfd].next;
	fcache[fcache[rfd].next].prev = fcache[rfd].prev;
	fcache[rfd].prev = NOFILE;
	fcache[rfd].next = freelst;
	freelst = rfd;
	return(OK);
}

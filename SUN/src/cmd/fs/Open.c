/* Open.c - Open */

#include "fs.h"

/*------------------------------------------------------------------------
 *  Open - open a file given its name and Xinu mode; return cache index
 *------------------------------------------------------------------------
 */
Open(name, mode)
char	*name;
int	mode;
{
	int	rfd;
	long	lseek();

	if (*name == NULLCH)
    		return(SYSERR);
	for (rfd = 0; rfd < MAXNFILES; ++rfd)
		if (strcmp(fcache[rfd].name, name) == 0)
			break;
	if (rfd < MAXNFILES && (fcache[rfd].mode & mode & FLRW) ==
		(mode&FLRW)) {	/* file already opened appropriately	*/
		fcache[fcache[rfd].prev].next = fcache[rfd].next;
		fcache[fcache[rfd].next].prev = fcache[rfd].prev;
	} else {
		if (rfd < MAXNFILES)	/* file must be reopened	*/
			Close(rfd);
		if (freelst == NOFILE)
			Close(OLDEST);
		rfd = freelst;
		if ((fcache[rfd].fd = openfile(name, mode)) < 0)
			return(SYSERR);
		if ((fcache[rfd].currpos=lseek(fcache[rfd].fd,0L,1))<0) {
			close(fcache[rfd].fd);
			return(SYSERR);
		}
		strcpy(fcache[rfd].name, name);
		fcache[rfd].mode  = mode;
		freelst = fcache[freelst].next;
	}
	fcache[rfd].prev = NEWEST;
	fcache[rfd].next = NOFILE;
	fcache[NEWEST].next = rfd;
	NEWEST = rfd;
	return(rfd);
}

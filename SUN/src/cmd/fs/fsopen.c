/* fsopen.c - fsopen */

#include "fs.h"

/*------------------------------------------------------------------------
 *  fsopen  -  handle open request and prepare reply
 *------------------------------------------------------------------------
 */
fsopen(fpacptr, flen)
struct	fpacket	*fpacptr;
int	flen;
{
	struct	stat	stinfo;
	struct	fphdr	*fptr;
	int	rfd;

	fptr = &fpacptr->fp_h;
	if ((rfd=Open(fptr->f_name, fptr->f_count)) < 0 ||
		fstat(fcache[rfd].fd, &stinfo) < 0)
		fptr->f_op = FS_ERROR;
	else
		fptr->f_pos = stinfo.st_size;
	return(FHDRLEN);
}

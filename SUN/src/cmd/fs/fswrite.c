/* fswrite.c - fswrite */

#include "fs.h"

/*------------------------------------------------------------------------
 *  fswrite  -  handle write request and prepare reply
 *------------------------------------------------------------------------
 */
fswrite(fpacptr, flen)
struct	fpacket	*fpacptr;
int	flen;
{
	struct	fphdr	*fptr;
	int	rfd;

	fptr = &fpacptr->fp_h;
	if ((rfd=Open(fptr->f_name, FLOLD|FLWRITE)) < 0
		|| Seek(rfd, fptr->f_pos) < 0
		|| (fptr->f_count=write(fcache[rfd].fd,
			fpacptr->fpdata, fptr->f_count)) < 0) {
		fptr->f_op = FS_ERROR;
	} else {
		fcache[rfd].currpos = fptr->f_pos += fptr->f_count;
	}
	return(FHDRLEN);
}

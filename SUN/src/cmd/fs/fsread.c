/* fsread.c - fsread */

#include "fs.h"

/*------------------------------------------------------------------------
 *  fsread  -  handle read request and prepare reply
 *------------------------------------------------------------------------
 */
fsread(fpacptr, flen)
struct	fpacket	*fpacptr;
int	flen;
{
	struct	fphdr	*fptr;
	int	rfd;
	int	n;

	fptr = &fpacptr->fp_h;
	if ((rfd=Open(fptr->f_name, FLOLD|FLREAD)) < 0
		|| Seek(rfd, fptr->f_pos) < 0
		|| (n=read(fcache[rfd].fd, fpacptr->fpdata,
			fptr->f_count)) < 0) {
		fptr->f_op = FS_ERROR;
		return(FHDRLEN);
	}
	fcache[rfd].currpos = fptr->f_pos += fptr->f_count = n;
	return(n + FHDRLEN);
}

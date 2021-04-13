/* fsclose.c - fsclose */

#include "fs.h"

/*------------------------------------------------------------------------
 *  fsclose  -  handle request to close a file
 *------------------------------------------------------------------------
 */
fsclose(fpacptr, flen)
struct	fpacket	*fpacptr;
int	flen;
{
	struct	fphdr	*fptr;
	int	rfd;

	fptr = &fpacptr->fp_h;
	if (Uncache(fptr->f_name) == SYSERR)
		fptr->f_op = FS_ERROR;
	return(FHDRLEN);
}
